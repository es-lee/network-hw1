HW1 Report
====
<p align=right>2013-11421 이은서</p>

### 1.Problem Statement
과제1은 간단한 채팅 프로그램을 만드는 것이다. 서버와 클라이언트를 모두
만들어야하며 TCP를 사용하기로 했다.

#### Specification
##### Server
- 서버는 여러 클라이언트에게 서비스를 제공해야 한다.
- 클라이언트가 deactivated인 경우 서버는 메시지를 저장했다가 클라이언트가
    activate 되었을 때 그 동안 받은 메시지를 전송해주어야 한다.

##### Client
- 클라이언트는 할당된 id로 로그온 할 수 있다.
- 메시지를 보낼 수 있다. 이 때 메시지 길이는 140자로 제한하기로 한다.
- 메시지를 받을 수 있다. 이 때 한 번 읽은 메시지는 다시 볼 수 없는 것으로 한다.

#### 계획
##### Server
- 여러 클라이언트를 동시에(?) 처리해야 하기 때문에 select()를 이용한 synchronous
    multiplexing 서버를 만들어 서비스하도록 한다.
- 사전에 등록되지 않은 id에 대해서는 로그온이 실패해야 한다.
- 클라이언트의 상태(activate, deactivate)에 대해 알아야 한다.
- 클라이언트가 deactivated일 때 메시지를 저장하기 위해 메시지 큐를 구현해야
    한다.

##### Client
- Activate된 상태일 때 자신에게 오는 메시지는 항상 받되, 읽기 명령이 있을 때만
    지난번에 읽은 마지막 메시지 다음부터 읽기 명령 전까지 받은 메시지를
    표시하도록 한다.
- 이를 위해서 스레드를 나누어 한 스레드에서는 자신에게 오는 메시지를 계속 받고,
    다른 스레드에서는 메시지 보내기 등의 명령을 처리하는 것으로 한다.

### 2.Implementation Details
#### Protocol Design
아래와 같이 프로토콜을 디자인했다.

##### Client to Server
1. Framing bits

1. action code
    - activate/logon : 0
    - message send : 1

2. log on protocol
    - [ action code | id ]

3. message send
    - [ action code | destination | source | message content ]

##### Server to Client
1. action code
    - activate : 0
    - message send 반응 : 1
    - message deliver : 2

2. log on 반응
    - log on 성공 : [ action code | 0 ]
    - log on 실패 : [ action code | 1 ]

3. message send 반응
    - reciever의 id가 valid한 경우 : [action code | 0]
    - reciever의 id가 unvalid한 경우 : [action code | 1]

4. message deliver
    - [ action code | source | message content ]

#### Functions
함수 설명들

#### Client Manual
- log on
    1. 로그온 하려면 0~9 사이의 한자리 숫자를 id로 입력하면 됩니다.
    1. 정상적으로 로그인 되면 `Log on success`가 콘솔에 나타납니다.

- 메시지 보내기
    1. 콘솔에 `s`를 입력합니다.
    1. 콘솔에 `R:`이 보이면 받을 사람의 id(0~9 사이의 한자리 숫자)를 입력합니다.
    1. 콘솔에 `M:`이 보이면 보내고 싶은 메시지를 입력합니다.

- 메시지 읽기
    1. 콘솔에 `r`를 입력합니다.
    1. 콘솔에 지금까지 받은 메시지들이 나타납니다.

- deactivate/log off
    1. 콘솔에 `q`를 입력합니다.

### 3.Results
블라블라

### 4.Discusion
블라블라
