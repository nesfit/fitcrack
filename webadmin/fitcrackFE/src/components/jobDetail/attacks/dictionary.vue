<!--
  * Author : see AUTHORS
  * Licence: MIT, see LICENSE
-->

<template>
  <fc-tile title="Dictionaries and rules">
    <v-data-table
      :headers="headers"
      :items="data.left_dictionaries"
      hide-default-footer
    >
      <template v-slot:item.dictionary.name="{ item: { dictionary } }">
        <router-link :to="{name: 'dictionaryDetail', params: { id: dictionary.id}}">
          {{ dictionary.name }}
          <v-icon 
            small
            color="primary"
          >
            mdi-open-in-new
          </v-icon>
        </router-link>
      </template>
      <template v-slot:item.dictionary.time="{ item: { dictionary } }">
        {{ $moment(dictionary.time).format('DD.MM.YYYY HH:mm') }}
      </template>
    </v-data-table>
    <template v-if="data.rulesFile.id !== null">
      <v-divider />
      <v-list-item class="px-2 py-1">
        <v-list-item-action class="pr-3 key">
          Rules:
        </v-list-item-action>
        <v-list-item-content class="text-right">
          <v-list-item-title>
            <router-link :to="{name: 'ruleDetail', params: { id: data.rulesFile.id}}">
              {{ data.rulesFile.name }}
            </router-link>
          </v-list-item-title>
          <span>
            Count: {{ data.rulesFile.count || 'not specified' }}
          </span>
        </v-list-item-content>
      </v-list-item>
    </template>
  </fc-tile>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "DictionaryDetail",
    components: {
      'fc-tile': tile
    },
    props: {
      data: {
        type: Object,
        default: () => {}
      }
    },
    data() {
      return {
        headers: [
          {
            text: 'Name',
            align: 'start',
            value: 'dictionary.name'
          },
          {text: 'Keyspace', value: 'dictionary.keyspace', align: 'end'},
          {text: 'Time', value: 'dictionary.time', align: 'end'}
        ]
      }
    },
  }
</script>

<style scoped>

</style>
