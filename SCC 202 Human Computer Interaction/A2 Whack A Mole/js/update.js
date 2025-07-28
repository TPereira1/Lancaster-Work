/*
Here are all the functions needing updates.
Read the coursework spec carefully and the in-line comments in the functions to understand what is needed here
*/
let chromeBugWorkaround = true;
const moleStats = {
  maxRounds:gameState.maxTrials,
  scale1: Math.round(gameState.maxTrials/3),
  scale2: Math.round(gameState.maxTrials/3),
  scale3: Math.round(gameState.maxTrials/3),
};

const getMolePosition = function (size) {
  //function to generate a new location for the mole
  //function returns an object with fields x and y i.e. {x:<val>, y:<val>}
  //function should only return values between -1 and 1 for both fields

  //I = 2^ID/2
  //x^2+y^2=(IW)^2
  //I=0.365
  let Xpos;
  let Ypos;
  switch(size){
    case 1.25:
      do{
        Xpos = getRandVal()
      }while(Math.pow(Xpos,2)>=Math.pow(0.365*1.25,2));
      Ypos = Math.sqrt(Math.pow(0.365*1.25,2)-Math.pow(Xpos,2));
      break;
      break;
    case 1.5:
      do{
        Xpos = getRandVal()
      }while(Math.pow(Xpos,2)>=Math.pow(0.365*1.5,2));
      Ypos = Math.sqrt(Math.pow(0.365*1.5,2)-Math.pow(Xpos,2));
      break;
    case 2.0:
      do{
        Xpos = getRandVal()
      }while(Math.pow(Xpos,2)>=Math.pow(0.365*2,2));
      Ypos = Math.sqrt(Math.pow(0.365*2,2)-Math.pow(Xpos,2));
      break;
  }
  
  if(Math.round(Math.random())==1){
    Ypos *=-1;
  }
  return { x: Xpos, y: Ypos };
};



const getMoleSize = function () {
  //function to generate a scale / size of the mole
  //function should return a single value between 1 and 2
  //you can use a discrete number of values for scale (e.g. three values 1.25, 1.5, 2)
  console.log("Max rounds", moleStats.maxRounds);
  console.log("Round", moleStats.scale1 + moleStats.scale2 + moleStats.scale3);
  if (gameState.trialCnt < gameState.maxTrials) {
    switch([1.25, 1.5, 2.0][Math.floor(Math.random() * 3)]){
      case 1.25:
        if(moleStats.scale1 > 0){
          moleStats.scale1--;
          return 1.25;
        }
        else{
          return getMoleSize();
        }
        break;
      case 1.5:
        if(moleStats.scale2 > 0){
          moleStats.scale2--;
          return 1.5;
        }
        else{
          return getMoleSize();
        }
        break;
      case 2.0:
        if(moleStats.scale3 > 0){
          moleStats.scale3--;
          return 2.0;
        }else if(moleStats.scale1 == 0 && moleStats.scale2== 0){
          return 2.0;
        }
        else{
          return getMoleSize();
        }
        break;
    }
  }
  return [1.25, 1.5, 2.0][Math.floor(Math.random() * 3)];
};

const getTimeout = function () {
  //function that generates the time available for the user to click on the mole
  //function returns a value in milliseconds
  return (3000-gameState.trialCnt*40);
};

const getCSVDataLine = function (obj, pid) {
  //function that converts each log entry into a single comma separated string
  //you are expected to convert all timestamps into computed values,
  return [
    pid,
    obj.tid,
    obj.moleStats,
    obj.userMoveStart,
    obj.userEnterMole,
    obj.userClickMole - obj.rndDelayEnd,
    obj.hitSuccess,
  ].join(",");
};
