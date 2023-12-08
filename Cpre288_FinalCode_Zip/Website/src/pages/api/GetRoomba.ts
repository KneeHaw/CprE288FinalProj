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
    const xata = getXataClient();

    const records = await xata.db.roomba
      .select(["id", "latitude", "longitude", "degrees", "xata.createdAt"])
      .getAll();

    if (records.length === 0 || records === undefined) {
      return new Response("No Roomba Records Found", { status: 311 });
    }
    const latestRecord = records.reduce((prev, current) =>
      prev.xata.createdAt > current.xata.createdAt ? prev : current
    );

    return new Response(JSON.stringify(latestRecord), { status: 200 });
  } catch (error) {
    console.error("GetRoomba.ts: Error Getting the Roomba Path: ", error);
    return new Response(error.message, { status: 500 });
  }
};
