// author: Conner Ohesorge (2023) | conneroh.com
import { defineConfig } from "astro/config";
import tailwind from "@astrojs/tailwind";
import svelte from "@astrojs/svelte";
import react from "@astrojs/react";
import vercel from "@astrojs/vercel/serverless";

import compress from "astro-compress";

export default defineConfig({
  integrations: [
    tailwind(),
    svelte(),
    react(),
    compress({
      CSS: true,
      HTML: true,
      Image: true,
      JavaScript: true,
      SVG: true,
    }),
  ],
  output: "server",
  adapter: vercel(),
});
