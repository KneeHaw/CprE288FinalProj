<script lang="ts">
  // author: Conner Ohnesorge (2023) | conneroh.com

  // import the onMount function
  import { onMount } from "svelte";
  // import the recipt component
  import Recipt from "./Recipt.svelte";
  // import the recipt footer component
  import ReciptFooter from "./ReciptFooter.svelte";

  // Defines the data variable (Holds the order data) as an empty array initially
  let data = [];

  // Defines the Mount function (Running when the component is mounted)
  onMount(async () => {
    setInterval(GetOrderData, 1000);
  });

  /**
   * Gets the order data from the server
   **/
  async function GetOrderData() {
    const response = await fetch("/api/GetOrders");
    data = await response.json();
  }
</script>

<!-- The Svelte HTML for the component, OrderMenu -->
{#if data.length === 0}
  <p>Add some orders to see them here</p>
{:else}
  {#each data as order}
    <div class="flex flex-col w-full min-h-screen">
      <main class="flex-1 p-4">
        <div class="flex items-center gap-4"></div>
        <div class="flex flex-col md:grid">
          <div
            class="md:col-span-4 lg:col-span-3 xl:col-span-4 flex flex-col gap-6"
          >
            <div
              class="rounded-lg border bg-card text-card-foreground shadow-sm"
              data-v0-t="card"
            >
              <h1
                class="font-semibold text-lg md:text-xl align-middle items-center self-center text-center"
              >
                Order #{order.id.toString().split("_")[1]}
                <br />
              </h1>
              <div
                class="font-semibold align-middle items-center self-center text-center"
              >
                <p>Ordered To: {order.house.name}</p>
                <p>
                  Ordered on: {order.xata.createdAt.toString().split("T")[0]}
                </p>
                <p>
                  Ordered at: {order.xata.createdAt
                    .toString()
                    .split("T")[1]
                    .split(".")[0]}
                </p>
              </div>
              <div class="flex flex-col space-y-1.5 p-6">
                <h3 class="text-2xl font-semibold leading-none tracking-tight">
                  Items
                </h3>
              </div>
              <div class="p-6">
                <div class="relative w-full overflow-auto">
                  <table class="w-full caption-bottom text-sm">
                    <thead class="[&amp;_tr]:border-b"
                      ><tr
                        class="border-b transition-colors hover:bg-muted/50 data-[state=selected]:bg-muted"
                        ><th
                          class="h-12 px-4 text-left align-middle font-medium text-muted-foreground [&amp;:has([role=checkbox])]:pr-0"
                        ></th><th
                          class="h-12 px-4 text-left align-middle font-medium text-muted-foreground [&amp;:has([role=checkbox])]:pr-0"
                          >Quantity</th
                        ><th
                          class="h-12 px-4 text-left align-middle font-medium text-muted-foreground [&amp;:has([role=checkbox])]:pr-0"
                          >Price</th
                        ></tr
                      ></thead
                    ><tbody class="[&amp;_tr:last-child]:border-0">
                      {#each order.goods as good}
                        <Recipt {good} />
                      {/each}
                    </tbody>
                  </table>
                </div>
                <ReciptFooter {order} />
              </div>
            </div>
          </div>
        </div>
      </main>
    </div>
  {/each}
{/if}
