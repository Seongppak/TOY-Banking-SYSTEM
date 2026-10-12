# 실행 검사 재현

검사에는 Python 3과 Visual Studio v145 C 빌드 도구가 필요합니다.
실제 프로젝트 EXE와 별도 함수 검사 드라이버 EXE를 먼저 빌드합니다.
드라이버는 테스트용 `rename`, `remove`, `fflush` 실패를 주입하며 실제 C 프로그램에는
해당 코드를 추가하지 않습니다. 테스트 드라이버 소스도 CP949입니다.

Visual Studio 개발자 명령 프롬프트에서 저장소 루트로 이동한 다음:

```cmd
msbuild "니마이 완성을 위한 프로젝트.slnx" /p:Configuration=Debug /p:Platform=x64
msbuild tests\test_driver.vcxproj /p:Configuration=Debug /p:Platform=x64
python tests\run_tests.py --exe "x64\Debug\니마이 완성을 위한 프로젝트.exe" --driver "tests\build\driver\test_driver.exe" --output "검사결과.json"
```

환경별 EXE 위치가 다르면 `--exe`에 빌드 출력의 실제 경로를 지정합니다.
실행 기록은 지정한 JSON에 저장합니다. `--output`에는 원본 파일을 지정하지 마십시오.
원본 TXT는 읽기와 해시 검사에만 사용하고, 모든 저장 시험은 시스템 임시 폴더의
새 복사본에서 실행합니다. 임시 폴더 위치는 마지막 출력에 표시됩니다.
복구 실패 테스트는 그 폴더의 `.bak`와 `.tmp`를 일부러 남겨 보존 여부를 검사합니다.

42개 고유 테스트와 재실행 준비 2회로 실행 검사 44개를 수행합니다.
실패가 있으면 스크립트가 오류로 종료합니다. PASS는 출력과 계좌 파일 상태에 대한
assert 검사를 통과한 경우에만 기록합니다. 실패 주입 검사는 실제 정전이나 디스크
고장을 발생시킨 검사가 아니며, 그 부분은 보고서에 구분하여 표시했습니다.

보고서 재현 화면은 실제 stdout에 전송한 입력을 프롬프트 위치에 삽입한 것입니다.
JSON의 `stdout`과 `inputs`는 이를 확인하기 위한 별도 원본 기록입니다.
각 임시 검사 폴더의 `stdout.cp949`에는 캡처한 바이트가 그대로 남습니다.

추가 수동 실행 검사에서 정상 3개 계좌의 마지막 줄 뒤에 `0x1A` 바이트와
불완전한 계좌 줄을 붙인 파일을 드라이버로 읽었습니다. `COUNT=-1 UNCHANGED=1`을
확인했으며 이 기록은 `reports/validation/build_summary.json`에 따로 남겼습니다.
`rb` 모드를 사용하므로 Windows의 Ctrl+Z 처리로 뒤의 자료가 사라지지 않습니다.
