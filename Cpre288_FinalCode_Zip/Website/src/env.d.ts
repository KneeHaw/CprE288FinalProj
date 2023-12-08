// author: Conner Ohnesorge (2023) | conneroh.com
/// <reference path="../.astro/types.d.ts" />
/// <reference types="astro/client" />

// Import the client from the xata package
interface ImportMetaEnv {
  readonly XATA_API_KEY: string;
  readonly XATA_BRANCH?: string;
}

// Import the types from the xata package
interface ImportMeta {
  readonly env: ImportMetaEnv;
}
