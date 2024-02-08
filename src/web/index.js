/**
 * Loading ROM
 */
let fileReader = new FileReader();

function loadRom() {
    let file = document.getElementById('romFile').files[0];
    fileReader.addEventListener('loadend', runRom);
    fileReader.readAsArrayBuffer(file);
}

function runRom(e) {
    let result = fileReader.result;
    const resultByteArray = new Uint8Array(result);
    const memFsFilename = "rom.nes"
    FS.writeFile(memFsFilename, resultByteArray);
    Module.ccall('loadRom', null, ['string'], [memFsFilename], {async: true});
}

/**
 * Module
 */
const CANVAS_ID = "canvas";

function printToLog(color, text) {
    let logElement = document.getElementById("log");
    let paragraphElement = document.createElement("p");
    paragraphElement.style = `color: ${color}; font-size: 12px`;
    paragraphElement.innerText = text;
    logElement.appendChild(paragraphElement);
}

var Module = {
    onRuntimeInitialized: function() {
        // Workaround to avoid DaisyUI styles being overwritten by style element inserted into head by Tailwind script 
        let daisyUiStylesheet = document.querySelector("link");
        document.querySelector("head").appendChild(daisyUiStylesheet);
    },
    
    canvas: (function(){
        let canvas = document.getElementById(CANVAS_ID);
        return canvas;
    })(),

    print: function(text) {
        printToLog("white", text);
    },

    printErr: function(text) {
        printToLog("red", text);
    }
}