<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <fc-tile title="Dictionaries and rules">
    <v-list single-line class="width100">
      <v-layout row wrap>
        <v-flex xs6>
          <v-list-tile class="px-2 py-1">
            <v-list-tile-content>
              <v-list-tile-title>Left dictionaries</v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
          <v-divider></v-divider>
          <v-data-table
          :headers="headers"
          :items="data.left_dictionaries"
          item-key="id"
          disable-initial-sort
          hide-actions
        >
          <template slot="items" slot-scope="props">
            <td>{{ props.item.dictionary.name }}</td>
            <td class="text-xs-right">{{ props.item.dictionary.keyspace }}</td>
            <td class="text-xs-right">{{ $moment(props.item.dictionary.time ).format('DD.MM.YYYY HH:mm') }}</td>
            <td class="text-xs-right">
              <v-tooltip top>
                <v-btn icon class="mx-0" :to="{name: 'dictionaryDetail', params: { id: props.item.dictionary.id}}" slot="activator">
                  <v-icon color="primary">link</v-icon>
                </v-btn>
                <span>Go to the dictionary page</span>
              </v-tooltip>
            </td>
          </template>
        </v-data-table>
        </v-flex>
        <v-flex xs6 class="borderLeft">
          <v-list-tile class="px-2 py-1">
            <v-list-tile-content>
              <v-list-tile-title>Right dictionaries</v-list-tile-title>
            </v-list-tile-content>
          </v-list-tile>
          <v-divider></v-divider>
          <v-data-table
          :headers="headers"
          :items="data.right_dictionaries"
          item-key="id"
          disable-initial-sort
          hide-actions
        >
          <template slot="items" slot-scope="props">
            <td>{{ props.item.dictionary.name }}</td>
            <td class="text-xs-right">{{ props.item.dictionary.keyspace }}</td>
            <td class="text-xs-right">{{ $moment(props.item.dictionary.time ).format('DD.MM.YYYY HH:mm') }}</td>
            <td class="text-xs-right">
              <v-tooltip top>
                <v-btn icon class="mx-0" :to="{name: 'dictionaryDetail', params: { id: props.item.dictionary.id}}" slot="activator">
                  <v-icon color="primary">link</v-icon>
                </v-btn>
                <span>Go to the dictionary page</span>
              </v-tooltip>
            </td>
          </template>
        </v-data-table>
        </v-flex>
      </v-layout>
      <v-divider></v-divider>
      <template v-if="data.rule_left !== ''">
        <v-list-tile class="px-2 py-1">
          <v-list-tile-action class="pr-3 key">
            Left rule:
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title class="text-xs-right">{{data.rule_left}}</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
        <v-divider></v-divider>
      </template>
      <v-divider></v-divider>
      <template v-if="data.rule_right !== ''">
        <v-divider></v-divider>
        <v-list-tile class="px-2 py-1">
          <v-list-tile-action class="pr-3 key">
            Right rule:
          </v-list-tile-action>
          <v-list-tile-content>
            <v-list-tile-title class="text-xs-right">{{data.rule_right}}</v-list-tile-title>
          </v-list-tile-content>
        </v-list-tile>
      </template>
    </v-list>
  </fc-tile>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "combinatorDetail",
    props: ['data'],
    components: {
      'fc-tile': tile
    },
    data() {
      return {
        headers: [
          {
            text: 'Name',
            align: 'left',
            value: 'name'
          },
          {text: 'Keyspace', value: 'keyspace', align: 'right'},
          {text: 'Time', value: 'time', align: 'right'},
          {text: 'Link to', value: 'name', sortable: false, align: 'right', width: "1"}
        ]
      }
    },
  }
</script>

<style scoped>
  .borderLeft {
    border-left: 2px solid #eee;
  }

</style>
