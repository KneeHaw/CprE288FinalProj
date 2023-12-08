// author: Conner Ohnesorge (2023)
import type { APIContext, APIRoute } from "astro";
import { getXataClient } from "../../xata";

/*
 * @type {APIRoute} Get Num Orders
 * @returns {Response} The Num Orders
 * @description This is the API Route for getting the number of orders
 * @example
 * ```astro
 * import { GET } from "./GetNumOrders.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * console.log(data);
 * ```
 * ```ts
 * const response = await fetch("/api/GetNumOrders");
 * const data = await response.json();
 * console.log(data);
 * ```
 */
export const GET: APIRoute = async ({clientAddress}: APIContext) => {
  try {
    // Get the Xata client
    const xata = getXataClient();

    // Select all orders
    const records = await xata.db.carts
      .select([
        "id",
        "client.ipaddress"
      ]). filter({
        "client.ipaddress": clientAddress,
      })
      .getAll();

    // new json object to return
    const returnObj = {
      num: records.length,
    };

    // Return a success response
    return new Response(JSON.stringify(returnObj), { status: 200 });
  }
  catch (error) {
    // Return an error response
    return new Response(error.message, { status: 500 })
  }
};
