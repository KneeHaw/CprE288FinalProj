// author: Conner Ohnesorge (2023)
import type { APIRoute } from "astro";

import { Resend } from "resend";

/**
 *  @type {APIRoute} Post The Email
 * @returns {Response} The Response Object with the Email from the Database
 * @throws {Response} The Response Object with the Error Message
 * @description This function posts the Email to the Database
 * @example
 * // Post the Email
 * const response = await fetch("/api/PostEmail");
 * // Get the Response Body
 * const body = await response.json();
 * // Log the Response Body
 * console.log(body);
 * // Log the Response Status
 * console.log(response.status);
 * // Log the Response Status Text
 * console.log(response.statusText);
 * @example
 **/
export const POST: APIRoute = async (request): Promise<Response> => {
  const resend = new Resend(import.meta.env.RESEND_API_KEY);

  const { email } = await (request as unknown as Request).json();
  try {
    const data = await resend.emails.send({
      from: "Roomba Rush <roombarush@resend.dev>",
      to: [email],
      subject: "Hello World",
      html: "<strong>It works!</strong>",
    });

    // Return the Response
    return new Response(JSON.stringify(data), { status: 201 });
  } catch (error) {
    // Log the Error
    console.error("PostEmail.ts: Error Posting the Email: ", error);
    // Return the Error Response
    return new Response(error.message, { status: 500 });
  }
};
