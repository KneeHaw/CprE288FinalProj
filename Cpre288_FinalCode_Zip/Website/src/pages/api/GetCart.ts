// author: Conner Ohnesorge (2023)
// author: Conner Ohnesorge (2023)
import type { APIContext, APIRoute } from "astro";
import { getXataClient } from "../../xata";

/*
 * @type {APIRoute} Get Num Orders
 * @returns {Response} The Num Orders
 * @description This is the API Route for getting the number of orders
 * @example
 * 
 * ```astro
 * import { GET } from "./GetNumOrders.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * console.log(data);
 * ```
 * 
 * ```ts
 * const response = await fetch("/api/GetNumOrders");
 * const data = await response.json();
 * console.log(data);
 * ```
 */
export const GET: APIRoute = async ({clientAddress}:APIContext) => {
  try {
    // Get the Xata client from the getXataClient function defined in xata.ts
    const xata = getXataClient();

    // get all orders from the database storing them in variable records
    const records = await xata.db.carts
      .select([
        "id",
        "good.id",
        "good.name",
        "good.photo.id",
        "good.photo.name",
        "good.photo.signedUrl",
        "good.photo.mediaType",
        "good.photo.size",
        "good.photo.enablePublicUrl",
        "good.photo.url",
        "good.calories",
        "good.type",
        "good.country_origin",
        "good.subtype",
        "client.id",
        "client.ipaddress",
        "client.name",
      ])
      .filter({
        "client.ipaddress": clientAddress,
      })
      .getAll();
    
    // Return a success response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return an error response
    return new Response(error.message, { status: 500 });
  }
};
