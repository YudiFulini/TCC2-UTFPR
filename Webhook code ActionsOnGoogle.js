const { conversation } = require('@assistant/conversation');
const functions = require('firebase-functions');
const admin = require('firebase-admin');


///-----------------------------------------------
//SET ACTIONS ON GOOGLE OBJECTS-------------------
//------------------------------------------------
admin.initializeApp({databaseURL: "https://tcc2-3e0a4-default-rtdb.firebaseio.com/"});
const db = admin.database();


///-----------------------------------------------
//SET ACTIONS ON GOOGLE OBJECTS-------------------
//------------------------------------------------
const app = conversation();


///-----------------------------------------------
//APP HANDLES-------------------------------------
//------------------------------------------------

//MOVE COMMAND
app.handle('MoveChair', conv => {
  var command = "move";
  var direction = conv.session.params.moveDirection;
  var distance = conv.session.params.moveDistance;
  var unity = conv.session.params.moveUnity;
  
  //convert s in ms and m in cm if needed
  if (distance < 1){
    if(unity == "s"){
      unity = "ms";
      distance = distance*1000;
    }
    if(unity == "m"){
      unity = "cm";
      distance = distance*100;
    }
  }
  
    conv.add("Movendo para "+ direction +".");
  
  if (direction == "trás")
    direction = "tras";
  
  var response = writeCommand(command, direction, distance, unity);
	conv.add(response);
});


//TURN COMMAND
app.handle('turnChair', conv => {
  var command = "turn";
	var direction = conv.session.params.turnDirection;
  var distance = "", unity = "";
    
  if(conv.session.params.turnDistance == undefined){
    distance = 90;
    unity = "graus";
  }else if(conv.session.params.turnUnity == undefined){
    distance = conv.session.params.turnDistance;
    unity = "graus";
  }else{
    distance = conv.session.params.turnDistance;
  	unity = conv.session.params.turnUnity;
  }
  
  //convert s in ms if needed
  if (distance < 1 && unity == "s"){
    unity = "ms";
		distance = distance*1000;
  }
  
  conv.add("Virando...");
  
  var response = writeCommand(command, direction, distance, unity);
  conv.add(response);
});


//STOP CHAIR
app.handle('stopChair', conv => {
  var command = "stop";
  var direction = "";
  var distance = "";
  var unity = "";
  
  conv.add("Parando...");
  var response = writeCommand(command, direction, distance, unity);
  conv.add(response);
});

app.handle('stopCancel', conv => {
  var command = "stop";
  var direction = "";
  var distance = "";
  var unity = "";
  
  conv.add("Parando a cadeira e finalizando o aplicativo...");
  var response = writeCommand(command, direction, distance, unity);
});


//SOFT RESET COMMAND
app.handle('softReset', conv => {
  var command = "reset";
  var direction = "";
  var distance = "";
  var unity = "";
  
  conv.add("Reiniciando a cadeira de rodas. Isso pode levar alguns segundos...");
  var response = writeCommand(command, direction, distance, unity);
	conv.add("Quando acender a luz verde, me chame novamente");
});


///-----------------------------------------------
//FUNCTIONS---------------------------------------
//------------------------------------------------
function writeCommand(command, direction, distance, unity) {
  var response = "";
  try{
    db.ref('/Wheelchairs/YudiChair').set({
    command: command,
    direction: direction,
    distance: distance,
    unity : unity
  });
    response = "Qual o próximo comando?";
  } catch (error){
    response = "Ops, tivemos um erro na conexão com o banco de dados.";
  }
  
  return response;
}

exports.ActionsOnGoogleFulfillment = functions.https.onRequest(app);

