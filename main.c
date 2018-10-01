#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <locale.h>
#include <string.h>


int callback(void *NotUsed, int argc, char **argv, char **azColName);



int main(void) {
	setlocale(LC_ALL,"Portuguese");
	
	unsigned int opc = 0;
	char nome[30];
	char *err = 0;
	
	while(opc <1 ){
		printf("Deseja Cadastrar um novo Cliente? \n");
		printf("1 - sim \n");
		printf("2 - não (mostra lista de clientes já existentes)\n");
		scanf("%u",&opc);
		
		switch(opc){
			case 1:
				//pede nome do cliente
				fflush(stdin);
				printf("qual o nome do cliente?\n");
				scanf("%[^\n]s",nome);
			
				
				
				//ponteiro para bd
				sqlite3 *banco;
				//inicia cnx			
				int cnx = sqlite3_open("teste.db",&banco);
				//verifk se tudo ok
				if(cnx == SQLITE_OK)
				{
					//verifica se nome nao esta vazio			
					if(strlen(nome)>0) 
					{
					   strupr(nome); //deixa nome com letras maiusculas
					  //prepara a query	
					  char *inserir = sqlite3_mprintf("insert into cliente(nome) values('%q');",nome);
					  
					  cnx = sqlite3_exec(banco,inserir,callback,0,&err);
					  
					  if(cnx == SQLITE_OK){
					  	printf("\n\nCliente cadastrado com sucesso !!!\n\n");
					  }else{
					  	printf("erro : %s",sqlite3_errmsg(banco) );
					  }
					
					}
				    
					
				}
				
				
			break;
			case 2:
				opc = 1; //muda var p/ sair do loop
				//e seguir a exec do programa
			break;
			
		}
		
	}
	
	
	//ponteiro para o bd
	sqlite3 *db;
	
	//arm cod de erro sql
	char *err_msg = 0;
	
	//abre cnx com db arq do diretório 
	//outra opção para sqlite3_open('memory:',&db) cria bd na memoria
	int link = sqlite3_open("teste.db", &db);
	
	//exibe informacoes sobre versao
	 //printf("%s\n", sqlite3_libversion());
	
	//verifica se tudo esta ok com a conexao
	if(link != SQLITE_OK)
	{
		printf("%s \n",sqlite3_errmsg(db));
		sqlite3_close(db);
	}
	
	//sql
	char *sql = "select * from cliente";
	printf("Dados dos clientes:\n\n") ;
	//executa  a query
	link = sqlite3_exec(db,sql,callback,0,&err_msg);
	
	
	//verifica a query fora executada com sucesso
	if(link != SQLITE_OK)
	{
		printf("Falha ao executar o select\n");
		printf("Erro SQL: ",err_msg);
		
		//libera memoria
		sqlite3_free(err_msg);
		//fecha cnx
		sqlite3_close(db);
		return 1;
	}
	
	
	
	return 0;
}


int callback(void *NotUsed, int argc, char **argv, char **azColName) {
    
    NotUsed = 0;
   
    for (int i = 0; i < argc; i++) {

        printf("%s = %s\t", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    
    printf("\n");
    
    return 0;
}
