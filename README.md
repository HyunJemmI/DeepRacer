<!-- #!/bin/bash

if [ "$0" == "$BASH_SOURCE" ]; then
    echo "이 스크립트는 실행이 아닌 소스로 사용되어야 합니다."
    echo "사용법: source $0"
    exit 1
fi-->

<!-- ->
## PS1 환경 변수를 변경함으로써 activated가 앞에 나오게끔 변경
## (activated) ubuntu@ip-172-31-41-14:~/carina$
export PS1="(activated) \[\e[1;32m\]\u\[\e[m\]@\[\e[1;34m\]\h\[\e[m\]:\[\e[1;36m\]\w\[\e[m\]\$ " -->

<!--cd `dirname $BASH_SOURCE`
ROOT_DIR=`pwd`

export PATH=$ROOT_DIR/scripts:$PATH
## sourcing 함으로써 scripts 폴더 내부 파일을 자동으로 실행하게끔 해준다.-->
# ESW Contest 2024 - Autonomous Driving

- 팀명 : 전농동CAR리나
- 팀원 : 신현지, 표기환, 송규현, 이현제, 이규호

## 처음 할일 [개인 노트북에서 작업할 때]
1. git clone [repo url]
1. git config --global user.name [user_name]
1. git config --global user.email [user_mail] -- local 에서만 제발
- 체크 :
1. git remote get-url origin //잘 확인 됐는 지 확인
1. git config --global user.name // 이름 확인
1. git config --global user.email // 이메일 확인
1. git

## 다음 할일 :
1. scripts 폴더에 ssh-key.pem 파일 추가하기 (이거 깃에 트랙하면 혼남)
1. chmod 400 ssh-key.pem
1. deepracer에서 ngrok을 켠다.(ngrok tcp 22)
1. scripts/ngrok-env에서 포트 번호 업데이트 (만약 ADDR도 달라졌으면 업데이트)
1. 여기서 terminal에 . ./activate # (. = source)
1. 배포를 하려면 deploy
1. ssh로 carina에 접속해야한다면 carina-ssh 치면된다(최대한 사용 x -- 서울 -> 버지니아 -> 일본 -> 한국)

---
## 작업한 내용 추가하기
1. git checkout -b "[branch name]" //"[branch name]"이란 이름으로 새 branch 생성
1. git status //현재 git branch 확인 (아래 단계마다 확인 가능)
1. git add . //현재 폴더 밑의 모든 변경 사항 추가
1. git commit -m "[commit name]" //[이런 사항으로 변경]이란 이름으로 커밋한다.
1. git push origin [branch name]
- Pull Requests 올리기
- LGTM(확인) 받으면 master branch에 merge가 됨

git merge origin master //다른 branch에 있을 때, master의 변경을 가져오기
git rebase master //다른 branch에 있을 때, master의 변경을 가져오기
git rebase --abort //rebase하려고 했는데 충돌이 날 때, 되돌리기

---
### [커밋 규칙]
- **[INITIAL]** — 리포지토리를 생성하고 최초로 파일을 업로드할 때 사용
- **[ADD]** — 새로운 파일을 추가할 때 사용
- **[UPDATE]** — 코드 변경 또는 개선 사항을 반영할 때 사용
- **[REFACTOR]** — 기존 코드를 리팩토링할 때 사용
- **[FIX]** — 잘못된 링크 수정, 필요한 모듈 추가 및 삭제할 때 사용
- **[REMOVE]** — 파일을 제거할 때 사용
- **[STYLE]** — UI/UX 또는 포맷 관련 비기능적 변경사항을 적용할 때 사용

