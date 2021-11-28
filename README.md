CONTROLE DE MOVIMENTAÇÃO DE CADEIRA DE RODAS POR COMANDOS DE VOZ UTILIZANDO GOOGLE ASSISTANT  

-----  
[ESTE REPOSITÓRIO AINDA ESTÁ EM FASE DE DESENVOLVIMENTO, PORTANTO ALGUNS PASSOS E TUTORIAIS PODEM ESTAR FALTANTES]  
  
-----  
INTRODUÇÃO  
- Este repositório leva em consideração que o visitante leu o TCC e veio em busca dos códigos para utilizar como referência num projeto próprio, ou com a intenção de dar continuidade neste projeto.  
- Este repositório têm como objetivo compartilhar os códigos que foram programados durante o desenvolvimento do projeto, para que o visitante possa utilizar como referência ao desenvolver o próprio projeto. Para aprender a como programar no Actions on Google, a utilizar o Firebase Realtime Database e utilizar a biblioteca do banco de dados no ESP32, consultar as REFERÊNCIAS disponibilizadas ao final deste repositório.  
- O tópico Referência providencia tutoriais e documentações importântes ensinando a utilizar as ferramentas que foram utilizadas para o desenvolvimento do projeto.  
  
-----  
1. ACTIONS ON GOOGLE
- Primeiramente, assistir ao vídeo na referência para melhor entendimento de como programar conversações, criar scenes, intents, types e chamar webhook.  
- Criar o seu projeto da actions on google. (https://console.actions.google.com/u/0/)
- Criar os types, scenes, intents, types, dentre outros do seu projeto. 
- Em webhook, escolher "Inline Cloud Functions" e realizar o primeiro deploy, isto deve criar um projeto para o usuário no Firebase.
- Entrar no (https://console.firebase.google.com/u/0/), entrar no projeto com o mesmo nome que criou para o Actions on Google, criar um Realtime Database e copiar a URL da sua database. 
- Baixar o código em "Webhook code ActionOnGoogle.js" disponibilizado neste repositório.  
- Colar o código em WEBHOOK -> index.js do seu projeto no actions on google.  
- Mudar a URL para a URL do seu projeto do Realtime Database.
- Mudar a path para a parte que você quer que os dados sejam upados dentro do Realtime Database.
- Clicar em SAVE FULLFILLMENT e dar DEPLOY FULLFILLMENT.
- Testar a conversação em TEST nas abas acima do Google Assistant. 
- Se tudo ocorreu com sucesso, é possível observar as mudanças no Realtime Database a partir do Firebase Console.
  
obs: a parte de programar a conversação, criação dos types e demais devem ser realizadas antes de colar o código no WEBHOOK. Prints com os types criados e demais detalhes serão upados para o depositório, porém infelizmente não há como copiar e colar esta parte do desenvolvimento. Para informações sobre como desenvolver, consultar o vídeo.

-----  
2. ESP32
- [continuar].
  
-----  
REFERÊNCIAS  
  
- ACTIONS ON GOOGLE - Para aprender a programar no Actions on Google, assistir o seguinte vídeo:https://www.youtube.com/watch?v=Z1hxvniJ18s&list=WL&index=8&ab_channel=GoogleDevelopers. Para mais detalhes, buscar a documentação do Google Assistant:
https://developers.google.com/assistant.  
  
  
- BIBLIOTECA ESP32 PARA UTILIZAR O FIREBASE REALTIME DATABASE - Para utilizar a biblioteca do Firebase Realtime Database no ESP32, ler o GITHUB do criador da biblioteca no URL: https://github.com/mobizt/Firebase-ESP-Client.  
  
  
- FIREBASE REALTIME DATABASE DOCUMENTAÇÃO: https://firebase.google.com/docs/database  
  
  
