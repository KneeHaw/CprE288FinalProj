// author: Conner Ohnesorge (2023)
import { getXataClient } from "../../xata";
import type { APIContext, APIRoute } from "astro";
/**
 * @type {APIRoute} Post The Obstacle
  * @param {APIContext} context The API Context
  * @returns {Response} The Response Object with the Obstacle from the Database
  * @throws {Response} The Response Object with the Error Message
  * @description This function posts the Obstacle to the Database
  * @example
  * // Post the Obstacle
  * const response = await fetch("/api/PostObstacle");
* // Get the Response Bodyo
* const body = await response.json();
* // Log the Response Body
* console.log(body);
* // Log the Response Status
* console.log(response.status);
* @example
**/
export async function POST({ request }: APIContext): Promise<Response> {
  try {
    // Get the Xata Client
    const xata = getXataClient();
    // Get the Request Body as JSON (the latitude and longitude)
    const { latitude, longitude } = await request.json();
    // Create the Record
    const record = await xata.db.obstacles.create({
      latitude: latitude,
      longitude: longitude,
    });
    return new Response(JSON.stringify(record), { status: 201 });
  } catch (error) {
    // Log the Error
    console.error("PostObstacle.ts: Error Posting the Obstacle: ", error);
    // Return the Error Response
    return new Response(error.message, { status: 500 });
  }
}
