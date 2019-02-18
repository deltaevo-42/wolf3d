<template>
	<div class="textures-list">
		<v-list two-line>
			<v-subheader>Textures</v-subheader>
			<template v-for="(item, k) in items">
				<v-list-tile :key="k">
					<v-list-tile-content>
						<v-list-tile-title v-html="item.name"></v-list-tile-title>
						<v-list-tile-sub-title v-html="item.type"></v-list-tile-sub-title>
					</v-list-tile-content>
					<v-list-tile-action>
						<v-btn icon ripple>
							<v-icon color="grey lighten-1">delete</v-icon>
						</v-btn>
					</v-list-tile-action>
				</v-list-tile>
			</template>
		</v-list>
	</div>
</template>

<script>
export default {
	name: "textures-list",
	data () {
		return {
			items: [
				{
					name: "Cobblestone",
					type: "NORMAL"
				},
				{
					name: "DODOLF",
					type: "SS"
				},
				{
					name: "Door",
					type: "DOOR"
				},
				{
					name: "Door",
					type: "DOOR"
				},
				{
					name: "Door",
					type: "DOOR"
				},
				{
					name: "Door",
					type: "DOOR"
				}
			]
		}
	}
}
</script>

<style>

</style>
