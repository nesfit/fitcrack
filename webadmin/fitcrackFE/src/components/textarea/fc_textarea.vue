<!--
   * Author : see AUTHORS
   * Licence: MIT, see LICENSE
-->

<template>
  <div class="textAreaCont">
    <v-row v-if="searchEnable">
      <v-text-field
        v-model="searchText"
        single-line
        hide-details
        label="Search"
        class=" ml-3 mb-1"
        clearable
        @click:clear="clearSearch"
      />
      <v-btn
        icon
        color="primary"
        class="mt-2"
        @click="search"
      >
        <v-icon>search</v-icon>
      </v-btn>
    </v-row>
    <v-divider />
    <div
      class="scrollCont"
      :style="style"
      :class="{editable: !readonly}"
    >
      <div
        v-if="noResults"
        class="text-center pt-4"
      >
        No result found.
      </div>
      <div
        v-else-if="loading"
        class="text-center pa-5"
      >
        <v-progress-circular
          slot="spinner"
          size="50"
          :width="3"
          indeterminate
          color="primary"
        />
      </div>
      <v-row
        v-else
        class="minheight200"
      >
        <div class="lineNumberCont pr-1 pl-3 pt-2">
          <p
            v-for="index in linesCount"
            :key="index"
            class="pa-0 ma-0"
          >
            {{ index }}
          </p>
        </div>
        <v-col>
          <textarea
            v-if="searching"
            v-model="searchData"
            class="pt-2 pl-1"
            :readonly="true"
            wrap="off"
          />
          <textarea
            v-else
            v-model="textareaData"
            class="pt-2 pl-1"
            ref="textarea"
            :readonly="readonly"
            wrap="off"
            @input="update"
            @blur="emmitBlur"
            @focus="emmitFocus"
          />
          <infinite-loading
            v-if="url && !searching"
            @infinite="loadData"
          >
            <v-progress-circular
              slot="spinner"
              size="50"
              :width="3"
              indeterminate
              color="primary"
            />
            <span slot="no-more">
              You reached the end of file.
            </span>
            <span slot="no-results">
              You reached the end of file.
            </span>
          </infinite-loading>
        </v-col>
        <slot name="after" />
        <div
          v-if="canRemoveLine"
          class="pt-2"
        >
          <p
            v-for="index in linesCount"
            :key="index"
            class="pa-0 ma-0"
          >
            <v-icon
              small
              color="error"
              class="removeLineBtn"
              @click="removeLine(index)"
            >
              close
            </v-icon>
          </p>
        </div>
      </v-row>
    </div>
  </div>
</template>

<script>
  import InfiniteLoading from 'vue-infinite-loading';
  export default {
    name: "FcTextarea",
    components: {
      InfiniteLoading,
    },
    props: {'maxHeight': String,
      'readonly': Boolean,
      'url': String,
      'searchEnable': Boolean,
      'canRemoveLine': Boolean,
      'value': {
        type: String,
        default: ''
      },
    },
    data() {
      return {
        data: '',
        page: 1,
        searching: false,
        searchText: '',
        searchData: '',
        noResults: false,
        loading: false,
        textareaData: ''
      }
    },
    computed: {
      linesCount: function () {
        if (this.searching) {
          return this.searchData.split('\n').length - 1
        } else {
          if (this.value) {
            return this.value.split('\n').length
          } else {
            return 1
          }
        }
      },
      style () {
        if (this.maxHeight) {
          return 'max-height: ' + this.maxHeight + 'px'
        }

      }
    },
    watch:{
      value: function(){
        this.textareaData = this.value
      }
    },
    mounted: function () {
      this.textareaData = this.value
    },
    methods: {
      focus () {
        this.$refs.textarea.focus()
      },
      emmitBlur() {
        this.$emit('blur', this.value)
      },
      emmitFocus() {
        this.$emit('focus', this.value)
      },
      update() {
        if (!this.readonly) {
          this.$emit('input', this.textareaData)
        }
      },
      loadData: function ($state) {
        this.axios.get(this.url,
          {
            params: {
              'page': this.page,
              'per_page': 100
            }
          }).then((response) => {

          if (response.data.status) {
            this.value += response.data.data
            this.page++
            $state.loaded()
          }
          else {
            $state.complete()
          }
        });
      },
      search: function() {
        this.noResults = false
        this.loading = true
        this.searching = true
        this.searchData = ''
        this.axios.get(this.url,
          {
            params: {
              'search': this.searchText
            }
          }).then((response) => {
            if (!response.data.status) {
              this.noResults = true
              this.searchData = ''
            } else {
              this.searchData = response.data.data
            }
            this.loading = false
          });
      },
      clearSearch: function () {
        this.searching = false
        this.noResults = false
      },
      removeLine: function (index) {
        var lines = this.textareaData.split('\n')
        lines.splice(index - 1, 1)
        this.textareaData = lines.join('\n')
        this.$emit('input', this.textareaData)
        this.$emit('blur', this.textareaData)
      }
    },
  }
</script>

<style scoped>
  textarea{
    width: 100%;
    height: calc(100% + 20px);
    color: #000;
    resize: none;
    overflow-x: scroll;
    overflow-y: hidden;
  }

  .lineNumberCont {
    color: #888888;
    border-right: 1px solid #eee;

    margin-left: 1px;
  }

  .textAreaCont {
    /*border: 2px solid rgba(0,0,0,.75);*/
    box-sizing: border-box;
  }

  .scrollCont {
    height: 100%;
    overflow: auto;
    background: #f1f1f1;
    min-height: 200px;
    font-family: monospace,monospace;
  }

  .minheight200 {
    min-height: 200px;
  }

  .editable {
    background: #fff;
  }

  .removeLineBtn {
    cursor: pointer;
  }

</style>
