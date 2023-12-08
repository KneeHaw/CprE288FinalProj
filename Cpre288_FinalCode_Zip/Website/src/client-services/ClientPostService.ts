// author: Conner Ohnesorge (2023) | conneroh.com

/**
 * Posts a new client to the server.
 * @param {string} clientAddress - The IP address of the client.
 * @returns {Promise<any>} The response data from the server.
 * @throws {Error} If there is an error posting the client.
 */
const postClient = async (clientAddress: string): Promise<any> => {
  // Call to the server to post the client
  const response = await fetch("/api/PostClient", {
    method: "POST",
    body: JSON.stringify({ clientAddress }),
    headers: {
      "content-type": "application/json",
    },
  });

  // If the response is not ok, throw an error
  if (!response.ok) {
    throw new Error(await response.text());
  }

  // Return the response
  return await response.json();
};

// Export the function
export default postClient;
