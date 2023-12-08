<script lang="ts">
  // author: Conner Ohnesorge (2023) | conneroh.com
  import { onMount } from "svelte";
  import GetGoodByIdService from "../../client-services/ClientGetGoodByIdService";
  export let good;
  let goodData = [];
  onMount(async () => {
    goodData = await GetGoodByIdService(good);
  });
</script>
  <!-- The Recipts Seen in the OrderMenu.svelte -->

<tr
  class="border-b transition-colors hover:bg-muted/50 data-[state=selected]:bg-muted"
  ><td class="p-4 align-middle [&amp;:has([role=checkbox])]:pr-0 font-medium">
    {#if goodData.name == undefined}
      Loading...
    {:else}
      {goodData.name}
    {/if}
  </td>
  <td class="p-4 align-middle [&amp;:has([role=checkbox])]:pr-0">1</td><td
    class="p-4 align-middle [&amp;:has([role=checkbox])]:pr-0"
    >
    {#if goodData.price == undefined}
      Loading...
    {:else}
    {goodData.price}
    {/if}
  </td
  ></tr
>
