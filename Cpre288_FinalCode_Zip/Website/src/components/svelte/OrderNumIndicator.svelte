<script lang="ts">
  // author: Conner Ohnesorge (2023) | conneroh.com

  // import the onMount function
  import { onMount } from "svelte";

  // Define the orderNum variable (Holds the number of orders)
  let orderNum = 0;

  // Defines the Mount function (Running when the component is mounted)
  onMount(async () => {
    setInterval(getNumOrders, 1000);
  });

  /**
   * Gets the number of orders from the server
   **/
  async function getNumOrders() {
    orderNum = await fetch("/api/GetNumGoods", { method: "GET" })
      .then((res) => res.json())
      .then((data) => data.num);
  }
</script>

<!-- Show the order number -->
{orderNum}
