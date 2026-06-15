import "./styles/main.css";
import "./styles/home.css";
import Game from "./scripts/game.js";

const astro = new Game();
astro.init({
  wasmLoaderScriptName: "astro.js"
});
