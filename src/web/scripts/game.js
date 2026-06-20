// 2026-06-13
//
// Holds code that initializes and sets-up a 
// webgl2 or webgl canvas.
//
// TODO list (features, improvements, and bug fixes)
// -------------------------------------------------
// [ ] Rethink WebGL initialization logic. We should
//     prioritize trying to initialize the wasm game
//     first. If that doesn't work, we move to a 
//     static picture solution. I think raylib only
//     works with WebGL2 so we don't need to check
//     for WebGL.

export default class Game 
{
  init(opts)
  {
    this.canvasWidth  = window.innerWidth;
    this.canvasHeight = window.innerHeight;

    const testCanvas = document.createElement("canvas");
    const gl2 = testCanvas.getContext("webgl2");
    const gl  = testCanvas.getContext("webgl");
    const contextType = gl2 ? 2 : gl ? 1 : 0;

    switch(contextType)
    {
      case 2:{ // WebGL2 Context
        this.initWasmGame(opts.wasmLoaderScriptName);
      }break;

      case 1:{ // WebGL Context
        this.initWebGLGame();
      }break;

      default:{
        console.log("[WEBGL] WebGL not supported.");
      }break;
    }

    this.canvas.addEventListener('contextmenu', (e) => {
      e.preventDefault();
    });
  }

  initWasmGame(wasmLoaderScriptName = "game.js")
  {
    console.log("[WEBGL] Initialized with webgl2 context.");
    this.canvas = document.createElement("canvas");
    this.canvas.id = "background-canvas";
    this.canvas.height = this.canvasHeight;
    this.canvas.width  = this.canvasWidth;
    document.body.append(this.canvas);

    window.Module = {
      canvas: this.canvas,
      arguments: [
        String(this.canvasWidth),
        String(this.canvasHeight), 
      ],
      webglContextAttributes: {
        majorVersion: 2,
        minorVersion: 0,
      }
    };

    const wasmLoader = document.createElement("script");
    wasmLoader.src = wasmLoaderScriptName;
    document.body.append(wasmLoader);
  }

  initWebGLGame()
  {
    console.log("[WEBGL] Initialized with webgl context.");
    this.canvas = document.createElement("canvas");
    this.canvas.id = "background-canvas";
    this.canvas.height = this.canvasHeight;
    this.canvas.width  = this.canvasWidth;
    document.body.append(this.canvas);
  }
}

