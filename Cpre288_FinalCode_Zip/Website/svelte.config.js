// author: Conner Ohnesorge (2023) | conneroh.com
import { vitePreprocess } from '@astrojs/svelte';

export default {
	preprocess: vitePreprocess(),
	onwarn: (warning, handler) => {
		if (warning.code === 'a11y-click-events-have-key-events') return
		handler(warning)
	},
};
