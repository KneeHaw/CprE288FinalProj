<script>
  // author: Conner Ohnesorge (2023) | conneroh.com
  import { onMount } from "svelte";
  import GetGoodByIdService from "../../client-services/ClientGetGoodByIdService";
    import OrderMenu from "./OrderMenu.svelte";
  export let order;
  const goods = order.goods;
  const goodData = [];
  let totalCost = 0;
  let status = "Preparing";
  onMount(async () => {
    goods.forEach(async (good) => {
      const goodie = await GetGoodByIdService(good);
      goodData.push(goodie);
      totalCost += goodie.price;
    });
    setInterval(updateStatus, 2000);
  });

  async function updateStatus() {
    await fetch("/api/GetOrderStatus", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({
        id: order.id,
      }),
    });
  }
</script>

<!-- The Footer of the Recipts Seen in the OrderMenu.svelte -->
<div class="flex flex-col space-y-1.5 p-6">
  <h3 class="text-2xl font-semibold leading-none tracking-tight">Total Cost</h3>
</div>
<div class="p-6 grid gap-4">
  <div class="flex items-center font-medium">
    <div>Total</div>
    <div class="ml-auto">{totalCost}</div>
  </div>
</div>

<div class="flex flex-col space-y-1.5 p-6">
  <h3 class="text-2xl font-semibold leading-none tracking-tight">Status</h3>
</div>
{#if status == "Preparing"}
  <div class="p-6 text-lg bg-yellow-300 rounded-full text-black">
    <div>Preparing</div>
  </div>
{:else if status == "Ready"}
  <div class="p-6 text-lg bg-red-300 rounded-full text-black">
    <div>Ready</div>
  </div>
{:else if status == "Delivering"}
  <div class="p-6 text-lg bg-blue-300 rounded-full text-black">
    <div>Delivering</div>
  </div>
{:else if status == "Delivered"}
  <div class="p-6 text-lg bg-orange-300 rounded-full text-black">
    <div>Delivered</div>
  </div>
{:else if status == "Cancelled"}
  <div class="p-6 text-lg bg-gray-300 rounded-full text-black">
    <div>Cancelled</div>
  </div>
{:else}
  <div class="p-6 text-sm"><div>Unknown</div></div>
{/if}
