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
    Module.ccall('loadRom', null, ['string'], [memFsFilename], { async: true });
}

/**
 * Module
 */
const CANVAS_ID = "canvas";

var Module = {
    onRuntimeInitialized: function() {
        // Workaround to avoid DaisyUI styles being overwritten by style element inserted into head by Tailwind script 
        let daisyUiStylesheet = document.querySelector("link");
        document.querySelector("head").appendChild(daisyUiStylesheet);
        Module.ccall('run', null, null, null);
    },
    
    canvas: (function(){
        let canvas = document.getElementById(CANVAS_ID);
        return canvas;
    })()
}