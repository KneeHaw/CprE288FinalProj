// author: Conner Ohnesorge (2023) | conneroh.com

/**
 * @typedef {JSON} Order
 * @property {string} good
 * @property {string} name
 * @property {string} house
 **/
async function CreateOrder(
  good: string,
  name: string,
  house: string
): Promise<JSON> {
  return await fetch("/api/PostOrder", {
    method: "POST",
    headers: {
      "content-type": "application/json",
    },
    body: JSON.stringify({
      good: good,
      name: name,
      house: house,
    }),
  }).then((res) => res.json());
  
}

// Export the function
export default CreateOrder;
