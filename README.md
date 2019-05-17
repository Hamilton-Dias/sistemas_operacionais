# sistemas_operacionais

#primeira versão

#tem o socket criado
#bind, servidor rodando
#Espera o cliente conectar



#Segunda versão

#o servidor aceita vários clientes 
#e envia a data/hora para o mesmo



#Tercera versão

#Uma thread controla cada conexão



#Quarta versão

#"Mkdir", "ls -l", "rm -r" e "cd"
#funcionando


#Quinta Versao

#Todos os comandos implmentados


#COMO USAR COMANDOS
#mkdir nomepasta
#rmdir nomepasta
#cd diretorio
#ls
#touch nomearquivo.extensao
#rm nomearquivo
#echo "mesagem" >> nomearquivo.extensao
#cat nomearquivo

#Para rodar

#abra um terminal para rodar como o servidor
#gcc server.c -o worker -w -lpthread -w
#abra quantos terminais quiser para se conectar como cliente
# nc localhost 8888