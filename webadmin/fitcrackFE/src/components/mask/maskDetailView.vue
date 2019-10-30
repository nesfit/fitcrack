<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div>
    <v-breadcrumbs
      v-if="data != null"
      :items="
        [
          { text: 'Masks', to: { name: 'masks' }, exact: true },
          { text: data.name }
        ]"
      divider="/"
    />

    <v-container>
      <v-row justify="center">
        <fc-tile
          title="Mask set"
          :loading="data==null"
          class="mx-2 dictContentContainer mb-4"
        >
          <v-list
            v-if="data != null"
          >
            <v-list-item>
              <v-list-item-action>
                Name:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-right">
                  {{ data.name }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
            <v-divider />
            <v-list-item>
              <v-list-item-action>
                Added:
              </v-list-item-action>
              <v-list-item-content>
                <v-list-item-title class="text-right">
                  {{ $moment(data.time).format('DD.MM.YYYY HH:mm') }}
                </v-list-item-title>
              </v-list-item-content>
            </v-list-item>
          </v-list>
          <v-row class="mx-2">
            <v-col>
              <v-btn
                v-if="!edit"
                class="ma-0"
                outlined
                color="primary"
                @click="edit=true"
              >
                Edit <v-icon class="ml-2">
                  mdi-pencil
                </v-icon>
              </v-btn>
              <v-btn
                v-else
                class="ma-0"
                outlined
                color="primary"
                @click="saveMask"
              >
                Save <v-icon class="ml-2">
                  mdi-save
                </v-icon>
              </v-btn>
              <v-btn
                class="ma-0"
                outlined
                color="primary"
                :href="$serverAddr + '/masks/' + data.id + '/download'"
                target="_blank"
              >
                Download <v-icon class="ml-2">mdi-download</v-icon>
              </v-btn>
            </v-col>
          </v-row>
          <v-divider />
          <div
            v-if="data != null"
            class="dictContent pa-2"
            :class="{editing: edit}"
          >
            <code
              class="code elevation-0"
              :contenteditable="edit"
              @input="update"
            >{{ data.data }}</code>
          </div>
        </fc-tile>
      </v-row>
    </v-container>
  </div>
</template>

<script>
  import tile from '@/components/tile/fc_tile'
  export default {
    name: "MaskDetailView",
    components: {
      'fc-tile': tile
    },
    data: function () {
      return {
        data: null,
        edit: false,
        newData: ''
      }
    },
    mounted: function () {
      this.loadData()
    },
    methods: {
      loadData: function ($state) {
        this.axios.get(this.$serverAddr + '/masks/' + this.$route.params.id).then((response) => {
          this.data = response.data
        });
      },
      update:function(event){
        console.log('text updated')
        this.newData = event.target.innerText
      },
      saveMask: function () {
        if (this.newData === '')
          return

        this.axios.post(this.$serverAddr + '/masks/' + this.data.id + '/update', {
          "newMaskSet": this.newData
        }).then((response) => {
          console.log(response.data)
          this.edit=false
        })
      }
    }
  }
</script>

<style scoped>
  .dictContentContainer {
    max-width: 800px;
  }
  .dictContent {
    overflow: auto;
    background: #eee;
  }

  .dictContent.editing {

  }

  .code::before {
    display: none;
  }
  .code {
    width: 100%;
    background: transparent;
    color: #000;
  }
  .width100 {
    width: 100%;
  }

  .margintop5 {
    margin-top: 10px;
  }
</style>
