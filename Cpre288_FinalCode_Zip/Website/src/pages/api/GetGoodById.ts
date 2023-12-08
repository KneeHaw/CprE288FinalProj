// author: Conner Ohnesorge (2023)
// author: Conner Ohnesorge (2023)
import type { APIContext, APIRoute } from "astro";
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
export const POST: APIRoute = async ({request}: APIContext) => {
    try {
        // Get the Xata client
        const xata = getXataClient();
        // Select all goods
        const {id} = await request.json();

        // Get the record with the id
        const record = await xata.db.goods.read(id);

        // Return a success response in form of the records as a JSON string
        return new Response(JSON.stringify(record), { status: 200 });
    } catch (error) {
        // Return an error response
        return new Response(error.message, { status: 500 });
    }


}
