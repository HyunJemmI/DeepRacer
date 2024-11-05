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


