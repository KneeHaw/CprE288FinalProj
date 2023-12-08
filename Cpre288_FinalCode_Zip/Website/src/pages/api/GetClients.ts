// author: Conner Ohnesorge (2023)
// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";
import { getXataClient } from "../../xata";
/**
 * @type {APIRoute} Get Clients
 * @returns {Response} The Clients
 * @description This is the API Route for getting all clients
 * @example
 * ```astro
 * import { GET } from "./GetClients.ts";
 * const response = await GET(request);
 * const data = await response.json();
 * ```
 *
 * ```ts
 * const response = await fetch("/api/GetClients", { method: "GET" });
 * const date = await response.json();
 * console.log(data);
 * ```
 **/
export const GET: APIRoute = async (): Promise<Response> => {
  try {
    // Get the Xata client
    const xata = getXataClient();

    // Select all clients
    const records = await xata.db.clients
      .select(["id", "ipaddress", "house.id"])
      .getAll();

    // Return a success response
    return new Response(JSON.stringify(records), { status: 200 });
  } catch (error) {
    // Return an error response
    return new Response(error.message, { status: 500 });
  }
};
