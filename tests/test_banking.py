"""Run with Python from a Visual Studio Developer Command Prompt.

Builds the real sources with MSVC and tests in temporary directories only.
"""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / '니마이 완성을 위한 프로젝트'
SEED = (SOURCE / 'customer_info.txt').read_bytes()
TOTAL = 0

def check(condition, label):
    global TOTAL
    assert condition, label
    TOTAL += 1
    print('PASS:', label)

def run(exe, folder, text):
    result = subprocess.run([str(exe)], cwd=folder, input=text, encoding='utf-8',
                            capture_output=True, timeout=10)
    return result.returncode, result.stdout

def rows(folder):
    return [line.split() for line in (folder / 'customer_info.txt').read_text(encoding='utf-8').splitlines()]

def login(account=100152351124, password=1524):
    return f'1\n{account}\n{password}\n'

def main():
    if not shutil.which('cl'):
        raise SystemExit('Run from a Visual Studio Developer Command Prompt (cl required).')
    with tempfile.TemporaryDirectory(prefix='banking-tests-') as temporary:
        folder = Path(temporary)
        exe = folder / 'banking.exe'
        flags = ['cl', '/nologo', '/utf-8', '/W4', '/WX', '/I' + str(SOURCE)]
        subprocess.run(flags + [str(SOURCE / 'main.c'), str(SOURCE / 'bandking.c'), '/Fe:' + str(exe)], cwd=folder, check=True)
        harness = folder / 'file_checks.exe'
        subprocess.run(flags + [str(ROOT / 'tests/file_checks.c'), str(SOURCE / 'bandking.c'), '/Fe:' + str(harness)], cwd=folder, check=True)
        subprocess.run([str(harness)], cwd=folder, check=True)
        check(True, 'file parsing, limits, BOM, malformed data, duplicates, save errors')
        data = folder / 'customer_info.txt'
        data.write_bytes(SEED)
        for account, pw, balance in [(100152351124, 1524, 10000), (100144523657, 1275, 25000), (100142566328, 1685, 100000)]:
            code, out = run(exe, folder, login(account, pw) + f'잔액조회\n{pw}\n아니오\n종료\n')
            check(code == 0 and f'잔액은 {balance}원' in out, f'initial account {account}: {balance}')
        code, out = run(exe, folder, login() + '입금\n500\n아니오\n출금\n1524\n200\n네\n김백제\n')
        check(code == 0 and rows(folder)[0][3] == '10300', 'deposit and withdrawal persisted')
        check(rows(folder)[1][3] == '25000' and rows(folder)[2][3] == '100000', 'other balances unchanged')
        check('입금 / 500원 / 거래 후 잔액 10500원' in out and '출금 / 200원 / 거래 후 잔액 10300원' in out, 'existing name check and log display')
        code, out = run(exe, folder, login() + '잔액조회\n1524\n아니오\n종료\n')
        check(code == 0 and '잔액은 10300원' in out, 'balance survives restart')
        code, out = run(exe, folder, '2\n홍길동\n100199999999\n1234\n3\n')
        check(code == 0 and '신규 사용자 등록이 완료' in out and rows(folder)[3] == ['100199999999', '1234', '홍길동', '0'], 'registration saved with zero balance')
        code, out = run(exe, folder, login(100199999999, 1234) + '잔액조회\n1234\n아니오\n종료\n')
        check(code == 0 and '잔액은 0원' in out, 'new user can log in after restart')
        before = data.read_bytes()
        code, out = run(exe, folder, '2\n중복\n100152351124\n3\n')
        check(code == 0 and '사용자 등록이 취소되었습니다.' in out and out.count('========== BANKING SYSTEM') == 2, 'duplicate immediately cancels to menu without password prompt')
        check(data.read_bytes() == before, 'duplicate preserves all existing data')
        code, out = run(exe, folder, '1\n100152351124\n0000\n3\n')
        check(code == 0 and '비밀번호가 틀립니다.' in out and '원하시는 작업을 입력' not in out, 'invalid login cannot access banking')
        for text in ['2\n' + '가' * 30 + '\n3\n', '2\n새이름\n9223372036854775808\n3\n', '2\n새이름\n44\n40000\n3\n', 'oops\n3\n']:
            code, out = run(exe, folder, text)
            check(code == 0 and data.read_bytes() == before, 'invalid input safely rejected: ' + text.splitlines()[1])
        code, out = run(exe, folder, login() + '입금\n9223372036854775807\n출금\n1524\n999999\n종료\n')
        check(code == 0 and data.read_bytes() == before, 'deposit overflow and excessive withdrawal rejected')
        code, out = run(exe, folder, login() + '입금\nabc\n종료\n')
        check(code == 0 and data.read_bytes() == before, 'bad amount returns to business menu')
        (folder / 'customer_info.txt.tmp').mkdir()
        code, out = run(exe, folder, login() + '입금\n500\n종료\n')
        check(code == 1 and '거래가 취소되었습니다.' in out and data.read_bytes() == before, 'failed save rolls back deposit and preserves file')
        code, out = run(exe, folder, '2\n새이름\n45\n1234\n3\n')
        check(code == 1 and '사용자 등록이 취소되었습니다.' in out and data.read_bytes() == before, 'failed registration save preserves existing accounts')
        (folder / 'customer_info.txt.tmp').rmdir()
        code, out = run(exe, folder, login() + '입금\n')
        check(code == 0, 'EOF safely terminates and saves')
        data.write_text(''.join(f'{i} 1234 name 0\n' for i in range(1, 101)), encoding='utf-8')
        before = data.read_bytes()
        code, out = run(exe, folder, '2\n3\n')
        check(code == 0 and '최대 계좌 개수' in out and data.read_bytes() == before, '100 account registration limit')
        data.write_text('wrong data\n', encoding='utf-8')
        code, out = run(exe, folder, '3\n')
        check(code == 1 and data.read_text() == 'wrong data\n', 'malformed file fails without overwrite')
        data.unlink()
        code, out = run(exe, folder, '3\n')
        check(code == 1 and not data.exists(), 'missing file reports error without replacement')
        data.write_bytes(b'')
        code, out = run(exe, folder, '2\n홍길동\n46\n1234\n3\n')
        check(code == 0 and rows(folder) == [['46', '1234', '홍길동', '0']], 'empty file permits first registration')
    print(f'All {TOTAL} checks passed.')

if __name__ == '__main__':
    main()
