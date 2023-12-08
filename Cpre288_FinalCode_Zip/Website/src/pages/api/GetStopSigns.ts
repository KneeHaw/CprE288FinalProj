// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";

/**
 * @type {APIRoute} Get The Roomba Path
 * @returns {Response} The Response Object with the Roomba Path from the Database
 * @throws {Response} The Response Object with the Error Message
 * @description This function gets the Roomba Path from the Database
 * @example
 * ```astro
 * import { GET } from "./GetRoomba.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * ```
 *
 * ```ts
 * import { GET } from "./GetRoomba.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * ```
 *
 **/
export const GET: APIRoute = async (): Promise<Response> => {
  try {
    // Get the Xata Client
    const xata = getXataClient();

    // Get the Records
    const records = await xata.db.stop_signs
      .select(["id", "latitude", "longitude"])
      .getAll();

    // Return the Response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return the Error Response
    return new Response(error.message, { status: 500 });
  }
};
