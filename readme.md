# Projeto do Compilador em linguagem C

1. [Por que tem "makefile" no projeto?](#por-que-tem-makefile-no-projeto)
2. [Sobre os testes](#sobre-os-testes)

## Por que tem "makefile" no projeto?
Foi especificado que o projeto não deveria ter bibliotecas externas. Mas sem o make fica inviável compilar o projeto, pois o número de arquivos ".c" vai aumentar muito, e o comando para compilar na mão ficaria extremamente grande.

## Sobre os testes 
Optei por escrever testes, pois acredito que nenhum projeto deva ser feito sem eles. 
É **INACEITÁVEL** um projeto sem testes!

### Sobre o Unity (framework de testes)
O primeiro framework que achei para testar C é o Unity. Ele é meio incompleto (nem mockar ele mocka), mas só de fazer o básico de testes já ajuda muito.
Aparentemente na linguagem C você tem que baixar a biblioteca inteira e colocar no código. Então fiz isso. Você pode ver que em "/tests/libs/unity" tem todos os arquivos da biblioteca, que são referenciados nos arquivos de teste. No diretório "tests/unit" tem todos os testes unitários. Não confunda o "unit" de testes unitários com o "unity", que é o Framework. Também é importante dizer que o Unity framework não tem nada a ver com a Unity Game Engine

#### Como rodar os testes?
É bem simples. Primeiro, assegure-se de que está no diretório padrão do projeto. Em seguinda, use o comando:

``` console
make test
````