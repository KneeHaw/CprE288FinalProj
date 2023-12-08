// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";


/* 
 * @type {APIRoute} Get Goods
  * @returns {Response} The Goods
  * @description This is the API Route for getting all goods
  * @example
  * ```astro
  * import { GET } from "./GetGoods.ts";
  * const response = await GET(request);
  * const data = await response.json();
  * ```
  * ```ts
  * import { GET } from "./GetGoods.ts";
  * const response = await GET(request);
  * const data = await response.json();
  * ```
  */
export const GET: APIRoute = async (req) => {
  try {
    // Get the Xata client
    const xata = getXataClient();
    // Select all goods
    const records = await xata.db.goods
      .select([
        "id",
        "name",
        "photo.id",
        "price",
        "photo.name",
        "photo.signedUrl",
        "photo.mediaType",
        "photo.size",
        "photo.enablePublicUrl",
        "photo.url",
        "calories",
        "type",
        "provider.id",
        "provider.name",
        "provider.latitude",
        "provider.longitude",
        "provider.photo.id",
        "provider.photo.name",
        "provider.photo.signedUrl",
        "provider.photo.mediaType",
        "provider.photo.size",
        "provider.photo.enablePublicUrl",
        "provider.photo.url",
      ])
      .getAll();
    // Return a success response in form of the records as a JSON string
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return an error response
    return new Response(error.message, { status: 500 });
  }


}
