from pathlib import Path
import subprocess, json, hashlib, re, shutil
from datetime import datetime
import argparse, tempfile

parser=argparse.ArgumentParser()
parser.add_argument('--exe', required=True)
parser.add_argument('--driver', required=True)
parser.add_argument('--output', required=True)
args=parser.parse_args()
ROOT=Path(tempfile.mkdtemp(prefix='toy-banking-tests-'))
REPO=Path(__file__).resolve().parent.parent
PROJECT=REPO/'니마이 완성을 위한 프로젝트'
EXE=Path(args.exe).resolve()
DRIVER=Path(args.driver).resolve()
BASE=(PROJECT/'customer_info.txt').read_bytes()
hashes={str(p.relative_to(REPO)):hashlib.sha256(p.read_bytes()).hexdigest() for p in REPO.rglob('customer_info.txt')}
RUNS=ROOT/'test_runs'/datetime.now().strftime('%Y%m%d_%H%M%S')
RESULTS=[]
PROMPTS=re.findall(r'printf\("([^"\n]*:)"\);', (PROJECT/'main.c').read_bytes().decode('cp949'))
def account_bytes(count):
    return ''.join(f'{100000000000+i} 1234 시험 10000\n' for i in range(count)).encode('cp949')
def rows(data):
    return [s.split() for s in data.decode('cp949').splitlines()]
def run(id, feature, purpose, inputs, expected, prevention, *, data=BASE, driver=False, args=(), setup=None, check=None, folder=None):
    folder=folder or RUNS/id
    folder.mkdir(parents=True,exist_ok=True)
    if data is not None: (folder/'customer_info.txt').write_bytes(data)
    if setup: setup(folder)
    original_path=folder/'customer_info.txt'
    before=original_path.read_bytes() if original_path.exists() else None
    text='\n'.join(inputs)+'\n' if inputs else ''
    p=subprocess.run([str(DRIVER if driver else EXE),*map(str,args)],input=text.encode('cp949'),capture_output=True,cwd=folder,timeout=10)
    (folder/'stdout.cp949').write_bytes(p.stdout)
    output=p.stdout.decode('cp949',errors='strict').replace('\r\n','\n')
    file=folder/'customer_info.txt'
    after=file.read_bytes() if file.exists() else None
    record={'id':id,'feature':feature,'purpose':purpose,'precondition':before.decode('cp949',errors='replace') if before is not None else 'customer_info.txt 없음', 'inputs':inputs,'expected':expected,'prevention':prevention,'stdout':output,'stderr':p.stderr.decode('cp949',errors='replace'),'exit_code':p.returncode,'file_after':after.decode('cp949',errors='replace') if after is not None else '없음','file_data_unchanged':after==before,'method':'함수 검사 드라이버' if driver else '실제 프로그램에 CP949 표준 입력 전달'}
    record['auxiliary_files']={}
    for suffix in ['.tmp','.bak']:
        auxiliary=folder/('customer_info.txt'+suffix)
        record['auxiliary_files'][suffix]=auxiliary.read_bytes().decode('cp949') if auxiliary.is_file() else ('디렉터리' if auxiliary.is_dir() else '없음')
    try:
        check(output,after,p.returncode,folder)
        if driver and 'COUNT=-1' in output: must(after==before)
        record['verdict']='PASS'
    except Exception as e:
        record['verdict']='FAIL'; record['error']=str(e)
    # Captured stdout is unmodified. Reconstructed transcript only inserts sent input at known prompts.
    merged=''; offset=0
    for value in inputs:
        candidates=[(output.find(prompt,offset),prompt) for prompt in PROMPTS if output.find(prompt,offset)>=0]
        if not candidates: break
        at,prompt=min(candidates,key=lambda x:x[0]); end=at+len(prompt)
        merged+=output[offset:end]+value+'\n'; offset=end
    merged+=output[offset:]
    record['transcript']=merged
    (folder/'stdout.txt').write_text(output,encoding='utf-8')
    (folder/'stdin.txt').write_text(text,encoding='utf-8')
    RESULTS.append(record)
    print(id,record['verdict'],record.get('error',''))
    return folder
def must(condition):
    assert condition
def balance(b,n=10000): return int(rows(b)[0][3])==n
def normal(o,b,c,f): must(c==0 and len(rows(b))==3)
def nochange(o,b,c,f): must(c==0 and b==BASE)
def invalidfile(o,b,c,f): must('COUNT=-1 UNCHANGED=1' in o and c==1)
LOGIN=['1','100152351124','1524']
run('TC-01','read_file','정상 계좌 3개와 CP949 이름을 확인',[], 'COUNT=3, 김백제 10000, 이온조 25000, 최쿠농 100000','파일 누락 읽기 및 한글 손상',driver=True,check=lambda o,b,c,f:must('COUNT=3' in o and '김백제 10000' in o and '이온조 25000' in o and '최쿠농 100000' in o and b==BASE))
run('TC-02','read_file','파일 없음은 오류이며 새 파일을 만들지 않음',[], 'COUNT=-1, 배열 불변, 종료 코드 1, 파일 생성 안 함','없는 데이터의 정상 사용',data=None,driver=True,check=lambda o,b,c,f:must('COUNT=-1 UNCHANGED=1' in o and c==1 and b is None))
run('TC-03','read_file','빈 파일에서 첫 사용자를 등록할 수 있음',['2','홍길동','100199999999','1234','3'],'등록 계좌 1개, 홍길동, 초기 잔액 0','빈 파일과 오류 혼동',data=b'',check=lambda o,b,c,f:must('등록된 계좌가 없습니다.' in o and c==0 and rows(b)==[['100199999999','1234','홍길동','0']]))
run('TC-04','read_file','정상 첫 줄 뒤 누락된 두 번째 줄도 전체 거부',[], 'COUNT=-1, 배열 불변','일부만 로드한 데이터로 기존 파일 덮어쓰기',data=BASE.splitlines(keepends=True)[0]+b'100144523657 1275\n',driver=True,check=invalidfile)
run('TC-05','read_file','파일 내부 중복 계좌 전체 거부',[], 'COUNT=-1, 배열 불변','같은 계좌의 다른 잔액 혼용',data=BASE+BASE.splitlines(keepends=True)[0],driver=True,check=invalidfile)
run('TC-06','read_file','100번째 계좌까지 정상 읽기',[], 'COUNT=100, ACCOUNT 출력 100개','배열 경계의 정상 계좌 누락',data=account_bytes(100),driver=True,check=lambda o,b,c,f:must('COUNT=100' in o and o.count('ACCOUNT=')==100 and c==0))
run('TC-07','read_file','101개 파일은 전체 오류',[], 'COUNT=-1, 배열 불변','배열 범위 초과',data=account_bytes(101),driver=True,check=invalidfile)
run('TC-08','find_account 로그인','정상 계좌번호로 작업 메뉴 진입',LOGIN+['메인메뉴','3'],'작업 메뉴 표시 후 시작 메뉴로 복귀','정상 계좌 검색 실패',check=lambda o,b,c,f:must('원하시는 작업을 입력해주세요' in o and b==BASE and c==0))
run('TC-09','find_account 로그인','미등록 계좌는 비밀번호를 묻지 않음',['1','100100000001','3'],'계좌번호를 찾을 수 없습니다; 시작 메뉴 복귀','다른 계좌를 잘못 선택',check=lambda o,b,c,f:must('계좌번호를 찾을 수 없습니다.' in o and '비밀번호를 입력' not in o and b==BASE))
run('TC-10','check_password','올바른 로그인 및 잔액조회 비밀번호',LOGIN+['잔액조회','1524','아니오','3'],'비밀번호 확인 및 잔액 10000','정상 인증 거부',check=lambda o,b,c,f:must('비밀번호가 올바르게 입력되었습니다.' in o and '현재 귀하의 잔액은 10000원 입니다.' in o and b==BASE))
run('TC-11','check_password','로그인 비밀번호 오류 후 시작 메뉴 복귀',['1','100152351124','1111','3'],'비밀번호가 틀립니다; 작업 메뉴 미진입','인증 실패 후 거래 허용',check=lambda o,b,c,f:must('비밀번호가 틀립니다.' in o and '원하시는 작업을 입력해주세요' not in o and b==BASE))
run('TC-12','deposit','10000원 계좌에 5000원 입금',LOGIN+['입금','5000','아니오','3'],'잔액 15000, 나머지 계좌 불변','입금 계산 또는 대상 계좌 오류',check=lambda o,b,c,f:must('현재 잔액은 15000원' in o and balance(b,15000) and rows(b)[1:]==rows(BASE)[1:] and c==0))
run('TC-13','deposit','0원 입금 거부',LOGIN+['입금','0','메인메뉴','3'],'입금 금액이 올바르지 않습니다; 10000 유지','무의미한 거래 처리',check=lambda o,b,c,f:must('입금 금액이 올바르지 않습니다.' in o and b==BASE))
run('TC-14','deposit','음수 입금 거부',LOGIN+['입금','-5000','메인메뉴','3'],'입금 거부, 10000 유지','음수 입금으로 잔액 감소',check=lambda o,b,c,f:must('입금 금액이 올바르지 않습니다.' in o and b==BASE))
run('TC-15','withdraw','10000원 계좌에서 3000원 출금',LOGIN+['출금','1524','3000','아니오','3'],'잔액 7000, 다른 계좌 불변','출금 계산 오류',check=lambda o,b,c,f:must('현재 잔액은 7000원' in o and balance(b,7000) and rows(b)[1:]==rows(BASE)[1:]))
run('TC-16','withdraw','잔액 전액 10000원 출금',LOGIN+['출금','1524','10000','아니오','3'],'잔액 0','경계값 출금 거부 또는 음수 잔액',check=lambda o,b,c,f:must('현재 잔액은 0원' in o and balance(b,0)))
run('TC-17','withdraw','잔액 10000보다 큰 15000 출금 거부',LOGIN+['출금','1524','15000','메인메뉴','3'],'출금 거부, 10000 유지','음수 잔액 발생',check=lambda o,b,c,f:must('잔액보다 큽니다.' in o and b==BASE))
run('TC-18','신규 등록 save_file','새 계좌를 잔액 0으로 저장',['2','홍길동','100199999999','1234','3'],'기존 3개 보존, 신규 1개 0원','신규 정보 누락 및 기존 계좌 변경',check=lambda o,b,c,f:must('신규 사용자 등록이 완료되었습니다.' in o and rows(b)[:3]==rows(BASE) and rows(b)[3]==['100199999999','1234','홍길동','0']))
DUP='이미 등록된 계좌번호입니다.\n사용자 등록이 취소되었습니다.\n\n메인 메뉴로 돌아갑니다.\n'
run('TC-19','신규 등록 중복 검사','중복 계좌 즉시 취소하고 비밀번호 추가 질문 없음',['2','홍길동','100152351124','3'],'지정된 취소 메시지, 기존 계좌 및 잔액 불변','중복 계좌 생성 및 재입력 강제',check=lambda o,b,c,f:must(DUP in o and '등록할 비밀번호' not in o and b==BASE and c==0))
run('TC-20','신규 등록 최대 수','100개 등록된 상태에서 추가 등록 거부',['2','3'],'최대 계좌 개수 초과 메시지, 100개 유지','배열 초과 등록',data=account_bytes(100),check=lambda o,b,c,f:must('최대 계좌 개수를 초과' in o and '신규 사용자 이름' not in o and len(rows(b))==100))
run('TC-21','show_balance','서로 다른 계좌 잔액을 같은 실행에서 조회',LOGIN+['잔액조회','1524','아니오','1','100144523657','1275','잔액조회','1275','아니오','3'],'첫 계좌 10000, 두 번째 25000','공통 잔액 변수 사용에 의한 혼동',check=lambda o,b,c,f:must('현재 귀하의 잔액은 10000원' in o and '현재 귀하의 잔액은 25000원' in o and b==BASE))
run('TC-22','save_file 입금','입금 직후 EOF로 종료해도 갱신',LOGIN+['입금','5000'],'파일 잔액 15000, .tmp/.bak 없음','종료 시 입금 데이터 유실',check=lambda o,b,c,f:must(balance(b,15000) and not (f/'customer_info.txt.tmp').exists() and not (f/'customer_info.txt.bak').exists()))
run('TC-23','save_file 출금','출금 직후 EOF 종료 후 갱신',LOGIN+['출금','1524','3000'],'파일 잔액 7000','출금 데이터 유실',check=lambda o,b,c,f:must(balance(b,7000)))
f=RUNS/'TC-24'
run('TC-24-A','재실행 준비','첫 프로세스로 입금 저장',LOGIN+['입금','5000','아니오','3'],'15000 저장','재실행 준비 상태 검증',folder=f,check=lambda o,b,c,f:must(balance(b,15000)))
run('TC-24','재실행 잔액 유지','별도 프로세스에서 저장된 잔액 조회',LOGIN+['잔액조회','1524','아니오','3'],'15000 출력','메모리에만 저장되는 잔액',data=None,folder=f,check=lambda o,b,c,f:must('현재 귀하의 잔액은 15000원' in o and balance(b,15000)))
f=RUNS/'TC-25'
run('TC-25-A','재실행 준비','첫 프로세스로 신규 사용자 저장',['2','홍길동','100199999999','1234','3'],'계좌 4개 저장','재실행 준비 상태 검증',folder=f,check=lambda o,b,c,f:must(len(rows(b))==4))
run('TC-25','재실행 신규 계좌','별도 프로세스에서 신규 계좌 로그인 및 이름 확인',['1','100199999999','1234','잔액조회','1234','네','홍길동','3'],'0원 조회, 실명 확인, 잔액조회 로그','신규 계좌 또는 한글 이름 유실',data=None,folder=f,check=lambda o,b,c,f:must('현재 귀하의 잔액은 0원' in o and '실명 확인 되었습니다.' in o and len(rows(b))==4))
run('TC-26','save_file 실패','임시 경로를 디렉터리로 만들어 입금 저장 실패',LOGIN+['입금','5000','잔액조회','1524','네','김백제','3'],'거래 취소, 메모리/파일 10000, 입금 로그 없음, 종료 코드 1','실패를 성공으로 표시 또는 메모리 잔액 변경',setup=lambda f:(f/'customer_info.txt.tmp').mkdir(exist_ok=True),check=lambda o,b,c,f:must('거래가 취소되었습니다.' in o and '현재 귀하의 잔액은 10000원' in o and '1. 잔액조회' in o and '1. 입금' not in o and b==BASE and c==1))
run('TC-27','read_number','메뉴와 입금 위치에 문자 입력',['abc',*LOGIN,'입금','hello','메인메뉴','3'],'입력 오류 2회 이상, 데이터 불변','문자로 인한 반복문 멈춤 및 잘못된 숫자 사용',check=lambda o,b,c,f:must(o.count('입력값이 잘못 되었습니다.')>=2 and b==BASE and c==0))
run('TC-28','정수 범위 검사','long long와 short 범위 초과 입력',['9223372036854775808','2','홍길동','100199999999','32768',*LOGIN,'입금','9223372036854775808','메인메뉴','3'],'범위 초과 거부, 신규 등록 취소, 3개 및 잔액 보존','정수 오버플로 및 비밀번호 형 변환 오류',check=lambda o,b,c,f:must(o.count('입력값이 잘못 되었습니다.')>=2 and '비밀번호가 잘못되었습니다.' in o and b==BASE))
run('TC-29','read_text 이름 길이','CP949 20바이트 한글 10글자 거부',['2','가나다라마바사아자차','3'],'이름 오류, 등록 취소, 추가 계좌 없음','이름 배열 범위 초과',check=lambda o,b,c,f:must('이름이 잘못되었습니다.' in o and '등록할 계좌번호' not in o and b==BASE))
run('TC-30','save_log show_log','입금 출금 조회 각 작업 직후 이름 인증 및 로그 조회',LOGIN+['입금','5000','네','김백제',*LOGIN,'출금','1524','3000','네','김백제',*LOGIN,'잔액조회','1524','네','김백제','3'],'입금 5000/15000, 출금 3000/12000, 조회 12000 로그; 세션별 번호 1','로그 금액 또는 잔액 오류',check=lambda o,b,c,f:must('1. 입금 / 5000원 / 거래 후 잔액 15000원' in o and '1. 출금 / 3000원 / 거래 후 잔액 12000원' in o and '1. 잔액조회 / 현재 잔액 12000원' in o and balance(b,12000)))
run('TC-31','이체 기존 한계','차감만 하고 수취 계좌를 바꾸지 않음',LOGIN+['이체','1524','2000','네','김백제','3'],'송금 잔액 8000, 다른 계좌 불변, 차감 안내 및 로그','완성된 계좌 간 이체로 오인',check=lambda o,b,c,f:must('수취 계좌에 입금하지 않습니다.' in o and '1. 이체 / 2000원' in o and balance(b,8000) and rows(b)[1:]==rows(BASE)[1:]))
run('TC-32','save_file rename 1 실패','첫 rename 실패 주입',[],'SAVE=-1, 원본 보존, 임시 파일 제거','기존 파일 이동 실패 때 원본 삭제',driver=True,args=[1],check=lambda o,b,c,f:must('SAVE=-1' in o and b==BASE and not (f/'customer_info.txt.tmp').exists()))
run('TC-33','save_file rename 2 실패','교체 실패 후 복구 성공 주입',[],'SAVE=-1, 원본 복구, tmp/bak 없음','교체 실패 후 원본 유실',driver=True,args=[2],check=lambda o,b,c,f:must('SAVE=-1' in o and b==BASE and not (f/'customer_info.txt.bak').exists() and not (f/'customer_info.txt.tmp').exists()))
run('TC-34','save_file 복구 실패','교체와 복구 rename 실패 주입',[],'SAVE=-1, 원본 경로 없음, bak에 기존 데이터와 tmp에 새 데이터 보존','복구 실패 시 유일한 백업 삭제',driver=True,args=[3],check=lambda o,b,c,f:must('기존 파일 복구에 실패' in o and 'SAVE=-1' in o and b is None and (f/'customer_info.txt.bak').read_bytes()==BASE and balance((f/'customer_info.txt.tmp').read_bytes(),15000)))
run('TC-35','save_file 백업 삭제 실패','새 파일 저장 후 backup remove 실패 주입',[],'SAVE=0, 새 잔액 15000, 기존 bak 보존 및 경고','성공한 파일 저장 뒤 메모리만 잘못 되돌림',driver=True,args=[4],check=lambda o,b,c,f:must('SAVE=0' in o and '백업 파일을 지우지 못했습니다.' in o and balance(b,15000) and (f/'customer_info.txt.bak').read_bytes()==BASE))
run('TC-36','save_file fflush 실패','디스크 기록 실패를 fflush에 주입',[],'SAVE=-1, 원본 보존','쓰기 오류가 난 임시 파일로 교체',driver=True,args=[5],check=lambda o,b,c,f:must('SAVE=-1' in o and b==BASE and not (f/'customer_info.txt.tmp').exists()))
run('TC-37','입금 합계 범위','최대 잔액 계좌에 1원 입금',LOGIN+['입금','1','메인메뉴','3'],'입금 거부, LLONG_MAX 유지','합계 오버플로',data=BASE.replace(b'10000\r\n',b'9223372036854775807\r\n',1),check=lambda o,b,c,f:must('입금 금액이 올바르지 않습니다.' in o and balance(b,9223372036854775807)))
run('TC-38','출금 비정상 금액','0원 및 -1원 출금 거부',LOGIN+['출금','1524','0','출금','1524','-1','메인메뉴','3'],'거부 2회, 잔액 10000','음수 출금으로 잔액 증가',check=lambda o,b,c,f:must(o.count('출금 액수가 올바르지 않거나 잔액보다 큽니다.')==2 and b==BASE))
run('TC-39','신규 등록 저장 실패','임시 경로 디렉터리로 신규 저장 차단',['2','홍길동','100199999999','1234','1','100199999999','3'],'등록 취소, 신규 로그인 실패, 원본 3개 유지','저장하지 못한 계좌를 정상 등록',setup=lambda f:(f/'customer_info.txt.tmp').mkdir(exist_ok=True),check=lambda o,b,c,f:must('사용자 등록이 취소되었습니다.' in o and '계좌번호를 찾을 수 없습니다.' in o and b==BASE and c==1))
run('TC-40','read_line 긴 줄','256바이트를 넘는 잘못된 줄 전체 거부',[],'COUNT=-1, 배열 불변','잘린 입력을 다음 정상 줄로 취급',data=BASE+b'a'*300+b'\n',driver=True,check=invalidfile)
run('TC-41','read_line NUL','잔액 뒤 NUL과 숨겨진 데이터 거부',[],'COUNT=-1, 배열 불변','NUL 뒤의 잘못된 내용을 무시',data=BASE.splitlines(keepends=True)[0].rstrip()+b'\x00 extra\n',driver=True,check=invalidfile)
run('TC-42','read_file 추가 필드','정상 계좌 줄 뒤 다섯 번째 필드 거부',[],'COUNT=-1, 배열 불변','형식 오류의 조용한 허용',data=BASE.splitlines(keepends=True)[0].rstrip()+b' extra\n',driver=True,check=invalidfile)
assert all(hashlib.sha256((REPO/p).read_bytes()).hexdigest()==h for p,h in hashes.items())
Path(args.output).resolve().parent.mkdir(parents=True,exist_ok=True)
Path(args.output).resolve().write_text(json.dumps(RESULTS,ensure_ascii=False,indent=2),encoding='utf-8')
print('Temporary test copies:', ROOT)
print('RESULT',len(RESULTS),'PASS',sum(x['verdict']=='PASS' for x in RESULTS),'ORIGINAL HASHES UNCHANGED')
assert all(x['verdict']=='PASS' for x in RESULTS)
