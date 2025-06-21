# Projeto do Compilador em linguagem C

1. [Por que tem "makefile" no projeto?](#por-que-tem-makefile-no-projeto)
2. [Sobre os testes](#sobre-os-testes)
3. [Como rodar?](#como-rodar-o-projeto)

## Por que tem "makefile" no projeto?
Foi especificado que o projeto não deveria ter bibliotecas externas. Mas sem o make fica inviável compilar o projeto, pois o número de arquivos ".c" vai aumentar muito, e o comando para compilar na mão ficaria extremamente grande.

"Mas você poderia deixar tudo num mesmo arquivo .c grandão". Até poderia, mas ficaria muito desorganizado. É melhor deixar tudo separado, até porque isso pode facilitar o desenvolvimento de etapas futuras do compilador. 

## Sobre os testes 
Optei por escrever testes, pois acredito que nenhum projeto deva ser feito sem eles. 
É **INACEITÁVEL** um projeto sem testes!

### Sobre o Unity (framework de testes)
O primeiro framework que achei para testar C é o Unity. Ele é meio incompleto (nem mockar ele mocka), mas só de fazer o básico de testes já ajuda muito.
Aparentemente na linguagem C você tem que baixar a biblioteca inteira e colocar no código. Então fiz isso. Você pode ver que em "/tests/libs/unity" tem todos os arquivos da biblioteca, que são referenciados nos arquivos de teste. No diretório "tests/unit" tem todos os testes unitários. Não confunda o "unit" de testes unitários com o "unity", que é o Framework. Também é importante dizer que o Unity framework não tem nada a ver com a Unity Game Engine

### Como rodar os testes?
É bem simples. Primeiro, assegure-se de que está no diretório padrão do projeto. Em seguinda, use o comando:

``` console
make test
````

## Como rodar o projeto?
A maneira mais fácil de rodar o projeto é atraves de um devcontainer. 
1. (VSCode) Para isso, recomendo instalar a extensão [devcontainers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers). É bem fácil instalar e não tem enrolação.
2. Em seguida é necessário que você esteja com o docker. 
3. Clone o repositório
4. Abra com o VSCode
5. No VSCODE, aperte "F1" e procure a opção "Reopen in container":
![alt text](/docs-assets/image-2.png)
6. Aguarde até que o devcontainer esteja configurado. Isso pode levar vários minutos, uma vez que ele precisa baixar a imagem do container
7. Após isso você terá todo um ambiente configurado com gcc 10.2.1 instalado dentro do container. 
8 Utilize o comando ```make``` para compilar

Exemplo de comando de entrada ```make``` e saída no terminal:
```console
vscode ➜ /workspaces/cpp (main) $ make
gcc -Wall -Wextra -g -std=c11 -o main ./main.c config/options.c reader/reader.c
```