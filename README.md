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

처음 할일 :
1. git config --global user.name [user_name]
1. git config --global user.email [user_mail]

1. scripts 폴더에 ssh-key.pem 파일 추가하기 (이거 깃에 트랙하면 혼남)
1. chmod 400 ssh-key.pem
1. deepracer에서 ngrok을 켠다.(ngrok tcp 22)
1. scripts/ngrok-env에서 포트 번호 업데이트 (만약 ADDR도 달라졌으면 업데이트)
1. 여기서 terminal에 . ./activate # (. = source)
1. 배포를 하려면 deploy
1. ssh로 carina에 접속해야한다면 carina-ssh 치면된다(그럴일이 없으면 좋겠다)
