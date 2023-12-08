// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";


/*
 * @type {APIRoute} Get Businesses
  * @returns {Response} The Businesses
  * @description This is the API Route for getting all businesses
  * @example
  * ```astro
* import { GET } from "./GetBusinesses.ts";
* const response = await GET(request);
* const data = await response.json();
* ```
* 
* ```ts
* import { GET } from "./GetBusinesses.ts";
* const response = await GET(request);
* const data = await response.json();
* ```
*
*/
export  const  GET: APIRoute = async () => {
  try {
    // Get the Xata client
    const xata = getXataClient();

    // Select all businesses
    const records = await xata.db.businesses
      .select([
        "id",
        "latitude",
        "longitude",
        "photo.id",
        "photo.name",
        "photo.signedUrl",
        "photo.mediaType",
        "photo.size",
        "photo.enablePublicUrl",
        "photo.url",
        "name",
      ])
      .getAll();
    
    // Return a success response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return an error response
    return new Response(JSON.stringify({ error: error.message }), {
      status: 500,
    });
  }

};
