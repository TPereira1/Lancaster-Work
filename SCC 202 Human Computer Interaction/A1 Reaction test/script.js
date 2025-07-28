console.log("Hello 🌎");

const experimentState = {
  typetest:'',
  experimentActive: false,
  testActive: false,
  lastTimeColorChanged: -1,
  times: [],
  timeoutId: -1,
  Rcount: 0,
  errors: 0,
};
const colours = ["Red","Green","Blue","Yellow","Black","Purple","Cyan","Orange"];

const startExperiment = function (type) {
  //start the experiment, remove the data stats
  experimentState.experimentActive = true; //indicate start of experiment
  experimentState.typetest = type;
  document.getElementById("ColourButton").hidden = true;
  document.getElementById("TextButton").hidden = true;
  document.querySelector("#time").textContent = ""; //clear the values from prior runs
  document.querySelector("#count").textContent = "";
  document.querySelector("#mean").textContent = "";
  document.querySelector("#sd").textContent = "";
  document.querySelector("#errors").textContent = "";
  if(experimentState.typetest == "Colour"){
    document.querySelector("#instruction").textContent =  "Press 'SPACE' when Colour changes to Red! Press 'a' for results!";
  }else if(experimentState.typetest == "Text"){
    document.querySelector("#instruction").textContent =  "Press 'SPACE' when Text changes to Red! Press 'a' for results!";
  }
  
  //start the first trial
  const hndStimulus = document.querySelector("#text");
  hndStimulus.style.color = "black";
  document.getElementById("text").style.fontSize = "60px";
  hndStimulus.innerHTML = "Ready?";
  startTrial();
};
const startTrial = function () {
  const randomDelay = Math.floor(Math.random() * 3 + 2); // 1.6 - 3.2s
  experimentState.timeoutId = window.setTimeout(
    showStimulus,
    randomDelay * 800
  ); //setTimeout runs in millisechangeTextColorconds
  console.info(
    "INFO: Trial",
    experimentState.times.length,
    " started. Random delay:",
    randomDelay
  );
};

const showStimulus = function () {
  console.info("INFO: Stimulus shown.");
  changeTextColor();
  startTrial();
};

const stopTest = function () {
  console.info("INFO: User reaction captured.");
  let currTime = Date.now();
  let deltaTime = currTime - experimentState.lastTimeColorChanged;
  experimentState.times.push(deltaTime);
  document.querySelector("#time").textContent ="Time: "+ deltaTime + " ms";
  document.querySelector("#count").textContent = "Count: " + experimentState.times.length;
  experimentState.testActive = false;
  if(experimentState.times.length == 10){
    stopExperiment();
  }
};

let arr=[];

const stopExperiment = function () {
  clearTimeout(experimentState.timeoutId); //stop the timer
  const hndStimulus = document.querySelector("#text");
  hndStimulus.style.color = "red";
  hndStimulus.innerHTML = "END"; //indicate stop
  experimentState.testActive = false;
  document.querySelector("#instruction").textContent = "Select Test:"
  document.getElementById("ColourButton").hidden = false;
  document.getElementById("TextButton").hidden = false;

  let stats = computeStatistics(experimentState.times);

  document.querySelector("#count").textContent = "Count: " + stats.cnt;
  document.querySelector("#mean").textContent =
    "Mean: " + stats.mean.toFixed(2) + "ms";
  document.querySelector("#sd").textContent =
    "SD: " + stats.sd.toFixed(2) + "ms";
  document.querySelector("#errors").textContent = "Errors: " + experimentState.errors;
  arr = experimentState.times;
  experimentState.times = [];
  experimentState.errors = 0;
  experimentState.Rcount = 0;
  experimentState.experimentActive = false;
};

const computeStatistics = function (timeArr) {
  //to get mean, get sum of all trials and divide by number of trials m = sum(x)/cnt(x)
  const sums = timeArr.reduce((acc, num) => acc + num, 0);
  const meanDeltaTime = sums / timeArr.length;

  //standard deviation is  sqrt(sum(x-mean)^2/cnt(x))
  const squaredDiffs = timeArr.reduce(
    (acc, num) => (num - meanDeltaTime) ** 2 + acc,
    0
  );
  const standardDeviationTime = Math.sqrt(squaredDiffs / timeArr.length);

  return {
    sd: standardDeviationTime,
    mean: meanDeltaTime,
    cnt: timeArr.length,
  };
};

function changeTextColor() {
  const hndStimulus = document.querySelector("#text");
  hndStimulus.style.color = colours[Math.floor(Math.random() * 8)];
  hndStimulus.innerHTML = colours[Math.floor(Math.random() * 8)];
  if(Math.floor(Math.random() * 15<experimentState.Rcount)){
    if(experimentState.typetest == "Colour"){
      hndStimulus.style.color = "red";
    }else{
      hndStimulus.innerHTML = "Red";
    }
  }
  experimentState.Rcount++;
  
  if(hndStimulus.style.color == "red" && experimentState.typetest == "Colour"){//Colour test
    experimentState.testActive = true;
    experimentState.Rcount = 0;
  }
  else if(hndStimulus.innerHTML == "Red" && experimentState.typetest == "Text"){//Text test
    experimentState.testActive = true;
    experimentState.Rcount = 0;
  }
  else{
    experimentState.testActive = false;
  }
  experimentState.lastTimeColorChanged = Date.now();
}

const onKey = function (evt) {
  if (evt == null) {
    evt = window.event;
  }
  switch (evt.which || evt.charCode || evt.keyCode) {
    case 32: //space
      if (experimentState.testActive) {
        stopTest();
      }
      else{
        experimentState.errors ++;
      }
      break;
    case 65: //a
      if (experimentState.experimentActive) {
        stopExperiment();
      }
      break;
    //if here, it is not handled, you can extend as needed
    default:
      console.warn(
        "TBD?: Key down, unhandled",
        evt.which,
        evt.charCode,
        evt.keyCode
      );
  }
};

window.addEventListener("keydown", onKey);