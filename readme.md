# Sobre o compilador
Esse compilador não compila para execução. Na verdade, ele realiza um "processo de compilação". Ele foi escrito em linguagem C com o objetivo de compilar uma linguagem de programação definido no documento linkado a seguir: 
[Especificações dop Compilador](CompiladorEspecificacao.pdf)
Como é possível ver, o compilador foi um projeto do meu curso de graduação em Ciência da Computação (IFG). Ele fez parte da disciplinas de "Gramática e Compiladores".

# Índice
1. [Como compilar](#como-rodar-o-projeto)
2. [Detalhamento](#detalhamento)

# Como rodar o projeto?
O projeto foi configurado para ser compilador com o GCC. Caso queira, é possível executá-lo via Dev Container, visto que está configurado para suportar essa tecnologia. De qualquer forma, vamos para o passo a passo de execução.

1. Entre na pasta raiz do projeto e execute o comando `make`. Isso irá automaticamente compilar o projeto. 
2. Insira os arquivos com os códigos a ser compilados na pasta `src/files/`. O compilador só suporta um arquivo por vez, então a linguagem não suporta modularização. 
3. Execute o comando `./build/bin/core ./src/files/[nome do arquivo]`. Substitua o nome do arquivo no parâmetro de execução. Exemplo `./build/bin/core ./src/files/file2.txt`
Pronto, o compilador já deve ter executado sem problemas

# Detalhamento
Aqui possuímos 3 fases com compilação. São elas a léxica, sintática e semântica