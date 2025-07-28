# Hello Whack-A-Mole Experiment!

This is the starter template for the Whack-A-Mole Experiment.
You have to build certain aspects of the experiment

← If required, read up on the Javascript Tutorial here: [Tutorial](https://scc202-introductory-tutorial.glitch.me/index.html)

You should be aware of the following Glitch Features:  
← No need to save. While you develop your site, your changes will happen ✨ immediately in the preview window.  
← On the left you'll see the files that make up your site, including HTML, JavaScript, and CSS.  
← You are not expected to add any more files, rather update the relevant ones.

_Last updated: 25 Sep 2023_

## What's in this project?

← `README.md`: That's this file, you are reading it.

← `index.html`: This is the main web page for your site. The HTML defines the structure and content of the page using _elements_. You'll see references in the HTML to the JS and CSS files.

← `style.css`: CSS files add styling rules to your content. The CSS applies styles to the elements in your HTML page. The style rules also make the image move when you click it.

← `js/*.js`: Various Javascript files needed to produce a working game for the experiment. All edits needed to make the experiment functional as per specficiation are limited to _update.js_

Open each file and check out the comments (in gray) for more info.

## Try this next 🏗️

___Remix the website by clicking here: [Remix now](https://glitch.com/edit/#!/remix/whack-that-mole)!___  
Take a look in `js/update.js` and read the comments. Think about what needs to be changed here to achieve the requirements of the spec.


## Game Logic

The game uses [AFrame](https://www.aframe.io) as a basis for rendering the scene and providing the interactive/dynamic behaviour.  
There are two interactive elements (mole, center) which can be accessed via `document.querySelector(".mole") and document.querySelector(".center")`  
The experiment is built using an interactive event-model, i.e. when specific events occur, functions are called based on the game state  
The three types of events are:  
← Interaction events (when user clicks on mole/center or the 'mouse' enters/leaves mole/center)  
← Timer event: Either for a random delay prior to making the mole appear or a calculated delay before mole disappears  
← Log events: These events are used to update the logging of the experimental data  

The game state is maintained within `gameState.trialState` with the following values:  
← `_waitInit_`: Game start state, awaiting user to position mouse over the center. Center is blue in color.  
← `_waitStartClick_`: User has positioned mouse over center (which turns green), awaiting start of trial which occurs when user clicks on center  
← `_waitRandomDelay_`: User clicks on center, which turns silver. A random delay is generated, which ends with a timer event that triggers transition to next state  
← `_waitTrialEnd_`: This state is entered as random delay ends. A new position and scale is computed for the mole and it is shown at the target position, a computed timeout delay is scheduled  
← `_trialEnded_` : This state is entered when user clicks on mole or the trial times out. Center turns green, and user is expected to reposition mouse over center, awaiting start of next trial  

Logging is built upon 'logEvent' which includes a sequence step (as `evt.detail.el`), data (as `evt.detail.action`) and timestamp (as `evt.detail.timestamp`);
`gameState.gameStart` provides an initial timestamp. The difference between the two will give elapsed time (in ms) for each sequence step's occurance


### Normalization of coordinates
Normalized coordinates are used to side-step display device differences (i.e. values between [-1,1]) instead of actual scene coordinates/screen pixels.  
← To position the mole horizontally (x-axis), use values between -1 (left edge of screen) and 1 (right edget of screen)  
← To position the mole vertically (y-axis), use value between 0.73 (top of screen) and -0.73 (bottom of screen)  
← The normalized x,y position indicates the center of the mole which is a square target   
← To get the size of the mole in normalized coordinates, multiply the scale-factor used in 'js/update.js>getMoleSize();' by 0.1  

__NOTE:__ Using values between [-0.73,0.73] for y-axis ensures the entire mole is always visible for scale values up to 2x within the scene which has an aspect ratio of 4:3



← Aim to complete the experiment on the same machine for all users (or on a sibling device).


The browser console (WARNINGS, INFO, DEBUG, ERRORS) can be used to identify programming errors made while updating update.js and identify locations where edits are required,

![Glitch](https://cdn.glitch.com/a9975ea6-8949-4bab-addb-8a95021dc2da%2FLogo_Color.svg?v=1602781328576)

## Glitch! Support

[Glitch](https://glitch.com) is a friendly community where millions of people come together to build web apps and websites.

- Need more help? [Check out our Help Center](https://help.glitch.com/) for answers to any common questions.
