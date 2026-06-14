import { defineConfig } from '@rsbuild/core';

export default defineConfig({
  source: 
  {
    entry:
    {
      index: "./src/web/index.js"
    }
  },
  html: 
  { 
    title: "Ternt's Weblogs",
    template: "./src/web/templates/index.html" 
  }
});

