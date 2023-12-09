# This directory will contain the code made for the entire project.

## It will be chunked into three parts:
1. Cybot_C_code
    - C files
    - Header files
    - note: these need to be merged into a single directory for compilation, they are seprated for readability
2. Socket Program
    - subdirectory
3. Website Code
    - /website
    ```shell
    public/
        |- assets/ (Publicily loaded assets)
        |- favicon.svg (favicon for the site)
    src/
        |- assets/  (Assets stored on the server)
        |- client-services/ (Functions Useable by the client)
        |- components/ (UI components svelte/react/astro)
        |- layouts/ (Astro Layout Files)
        |- pages/ (Astro Pages Files)
        |- styles/ (CSS Style Files)
        |- env.d.ts (Environment Typescript File)
        |- xata.ts (Database File)
    README.md
    astro.config.mjs
    package.json
    svelte.config.js
    tailwind.config.mjs
    tsconfig.json
    ```
