# MySQL-C
MySQL을 연동한 C 프로젝트 
[선행학습](https://github.com/ParkYongJun03/MySQL)
- MySQL 설치
- WorkBench 설치
- SampleDB 테스트
- 쿼리문 실습

## MySQL-C (수정 - 32bit)
git만들때 꼭 저거 해주기 gitignore - VisualStudio

> ### MySQL - C Connector lib 설치 [MySQL Connector/C (Archived Versions) 링크](https://downloads.mysql.com/archives/c-c/)
> 32bit로 설치해 준다.
> 
> Connector 설치 경로 ~~`C:\DEV\Server\MySQL\MySQL Connector C 6.1`~~ `C:\DEV x86\Server\MySQL\MySQL Connector C 6.1`
> 
> C프로젝트(솔루션) 만들기 ->  프로젝트 속성페이지 ->
> 
>> <img src="https://user-images.githubusercontent.com/83456300/174921116-6e221455-3157-40af-a6cf-c6263baedc5f.png" width = "500">
>> 
>> 문자집합 -> `멀티바이트`, 플랫폼 -> `Win32`
>
> 
>> <img src="https://user-images.githubusercontent.com/83456300/174712762-f264847e-b0f0-4ea5-9aaa-35c729c50b0b.png" width="400">
>>
>> 포함 디렉터리 :
>> 
>> ~~`C:\DEV\Server\MySQL\MySQL Connector C 6.1\include;`~~ `C:\DEV x86\Server\MySQL\MySQL Connector C 6.1\include;`추가
>>  
>> 라이브러리 디렉터리 : ~~`C:\DEV\Server\MySQL\MySQL Connector C 6.1\lib;`~~ `C:\DEV x86\Server\MySQL\MySQL Connector C 6.1\lib;`추가
>>
> <img src="https://user-images.githubusercontent.com/83456300/174712340-57e67c53-90ff-456b-85ee-2d1682ff8536.png" width="400">


> ### 폰트 설정
> <img src="https://user-images.githubusercontent.com/83456300/174712113-9a156f31-58ce-4b98-a274-472957635a50.png" width="500">
>
> `/source-charset:euc-kr /execution-charset:euc-kr`
> 
> <img src="https://user-images.githubusercontent.com/83456300/174723285-705fad36-5be2-45d7-b88b-7835f5a3a64c.png" width="400">
> 
> 저장에 오른쪽 바 눌러서 인코딩하여 저장
> 
> <img src="https://user-images.githubusercontent.com/83456300/174922904-6558ce38-83b0-428d-adc5-6cf805716901.png" width="500">
> 
> 상단 바에 도구 -> 옵션 -> 텍스트 편집기 -> `시그니처 없는 UTF-8 자동 검색 체크 해제` (malfunction 요소)
> 




> [SQL Server](https://www.microsoft.com/ko-kr/sql-server/sql-server-downloads)
> 
> `C:\DEV\Server\SQL2019` 에 설치했음
> 
> 스크롤 내려서 Developer 버전(무료) 설치
> 
> 데이터베이스 엔진 서비스 체크
> 
> 클라이언트 도구 연결
> 
> 클라이언트 도구 이전 버전과의 호환성
> 
> 클라이언트도구 SDK
> [SSMS(workbench같은거)](https://docs.microsoft.com/ko-kr/sql/ssms/download-sql-server-management-studio-ssms?view=sql-server-ver16)
> `C:\DEV (x86)\Server\Microsoft SQL Server Management Studio 18`에 깔았음
