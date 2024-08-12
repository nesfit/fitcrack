<<<<<<<< HEAD:webadmin/fitcrackFE/dist/assets/markovView.681af2a0.js
import{t as l}from"./fc_tile.217bb9a5.js";import{f as d}from"./fileCreator.da04c38e.js";import{n as c}from"./index.d3f441ac.js";import"./dictionarySelector.1f9ad920.js";import"./numberFormat.773889d3.js";import"./selectorMixin.a647ee87.js";import"./fileUploader.8959d605.js";import"./vendor.6aa46313.js";var v=function(){var t=this,a=t.$createElement,e=t._self._c||a;return e("v-container",{staticClass:"max500"},[e("fc-tile",{staticClass:"ma-2",attrs:{title:"Markov chains",icon:t.$route.meta.icon}},[e("v-alert",{staticClass:"mb-0",attrs:{tile:"",text:"",type:"warning"}},[t._v(" Markov files must have a .hcstat2 extension. ")]),e("v-data-table",{attrs:{headers:t.headers,items:t.markovChains.items,loading:t.loading,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"Hcstats per page"}},scopedSlots:t._u([{key:"item.time",fn:function(o){var i=o.item;return[t._v(" "+t._s(t.$moment.utc(i.time).local().format("DD.MM.YYYY HH:mm"))+" ")]}},{key:"item.actions",fn:function(o){var i=o.item;return[e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(r){var n=r.on;return[e("a",t._g({attrs:{href:t.$serverAddr+"/markovChains/"+i.id,target:"_blank",download:""}},n),[e("v-btn",{attrs:{icon:""}},[e("v-icon",[t._v("mdi-file-download-outline")])],1)],1)]}}],null,!0)},[e("span",[t._v("Download")])]),e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(r){var n=r.on;return[e("v-btn",t._g({attrs:{icon:""},on:{click:function(_){return t.deleteMarkov(i)}}},n),[e("v-icon",{attrs:{color:"error"}},[t._v(" mdi-delete-outline ")])],1)]}}],null,!0)},[e("span",[t._v("Delete")])])]}}])}),e("div",{staticClass:"text-right pa-3"},[e("v-btn",{staticClass:"d-inline-block",attrs:{color:"primary",text:"",outlined:""},nativeOn:{click:function(o){o.stopPropagation(),t.dialog=!0}}},[t._v(" Add new ")])],1)],1),e("file-creator",{attrs:{title:"Add new HcStat","upload-url":this.$serverAddr+"/markovChains/add",working:t.working},on:{fileUploaded:t.loadMarkovChains,dictionarySelected:t.makeHcStatFromDictionary},model:{value:t.dialog,callback:function(o){t.dialog=o},expression:"dialog"}})],1)},m=[];const u={name:"MarkovView",components:{fileCreator:d,"fc-tile":l},data:function(){return{working:!1,dialog:!1,loading:!1,headers:[{text:"Name",align:"start",value:"name"},{text:"Added",value:"time",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}],markovChains:[]}},mounted:function(){this.loadMarkovChains()},methods:{loadMarkovChains:function(){this.dialog=!1,this.loading=!0,this.axios.get(this.$serverAddr+"/markovChains",{}).then(t=>{this.markovChains=t.data,this.loading=!1})},makeHcStatFromDictionary:function(t,a){this.working=!0,this.axios.post(this.$serverAddr+"/markovChains/makeFromDictionary",{dictionary_id:t,name:a}).then(e=>{this.working=!1,this.dialog=!1,this.loadMarkovChains()}).catch(e=>{this.working=!1})},deleteMarkov:function(t){this.$root.$confirm("Delete",`This will remove ${t.name} from your Markov chains. Are you sure?`).then(a=>{this.loading=!0,this.axios.delete(this.$serverAddr+"/markovChains/"+t.id).then(e=>{this.loadMarkovChains()})})}}},s={};var f=c(u,v,m,!1,h,"8e58174a",null,null);function h(t){for(let a in s)this[a]=s[a]}var b=function(){return f.exports}();export{b as default};
========
import{t as l}from"./fc_tile.83a0e997.js";import{f as d}from"./fileCreator.9a368800.js";import{n as c}from"./index.c11e1020.js";import"./dictionarySelector.6e2d3817.js";import"./numberFormat.773889d3.js";import"./selectorMixin.fb99fd96.js";import"./fileUploader.24c45618.js";import"./vendor.6aa46313.js";var v=function(){var t=this,a=t.$createElement,e=t._self._c||a;return e("v-container",{staticClass:"max500"},[e("fc-tile",{staticClass:"ma-2",attrs:{title:"Markov chains",icon:t.$route.meta.icon}},[e("v-alert",{staticClass:"mb-0",attrs:{tile:"",text:"",type:"warning"}},[t._v(" Markov files must have a .hcstat2 extension. ")]),e("v-data-table",{attrs:{headers:t.headers,items:t.markovChains.items,loading:t.loading,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"Hcstats per page"}},scopedSlots:t._u([{key:"item.time",fn:function(o){var i=o.item;return[t._v(" "+t._s(t.$moment.utc(i.time).local().format("DD.MM.YYYY HH:mm"))+" ")]}},{key:"item.actions",fn:function(o){var i=o.item;return[e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(r){var n=r.on;return[e("a",t._g({attrs:{href:t.$serverAddr+"/markovChains/"+i.id,target:"_blank",download:""}},n),[e("v-btn",{attrs:{icon:""}},[e("v-icon",[t._v("mdi-file-download-outline")])],1)],1)]}}],null,!0)},[e("span",[t._v("Download")])]),e("v-tooltip",{attrs:{top:""},scopedSlots:t._u([{key:"activator",fn:function(r){var n=r.on;return[e("v-btn",t._g({attrs:{icon:""},on:{click:function(_){return t.deleteMarkov(i)}}},n),[e("v-icon",{attrs:{color:"error"}},[t._v(" mdi-delete-outline ")])],1)]}}],null,!0)},[e("span",[t._v("Delete")])])]}}])}),e("div",{staticClass:"text-right pa-3"},[e("v-btn",{staticClass:"d-inline-block",attrs:{color:"primary",text:"",outlined:""},nativeOn:{click:function(o){o.stopPropagation(),t.dialog=!0}}},[t._v(" Add new ")])],1)],1),e("file-creator",{attrs:{title:"Add new HcStat","upload-url":this.$serverAddr+"/markovChains/add",working:t.working},on:{fileUploaded:t.loadMarkovChains,dictionarySelected:t.makeHcStatFromDictionary},model:{value:t.dialog,callback:function(o){t.dialog=o},expression:"dialog"}})],1)},m=[];const u={name:"MarkovView",components:{fileCreator:d,"fc-tile":l},data:function(){return{working:!1,dialog:!1,loading:!1,headers:[{text:"Name",align:"start",value:"name"},{text:"Added",value:"time",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}],markovChains:[]}},mounted:function(){this.loadMarkovChains()},methods:{loadMarkovChains:function(){this.dialog=!1,this.loading=!0,this.axios.get(this.$serverAddr+"/markovChains",{}).then(t=>{this.markovChains=t.data,this.loading=!1})},makeHcStatFromDictionary:function(t,a){this.working=!0,this.axios.post(this.$serverAddr+"/markovChains/makeFromDictionary",{dictionary_id:t,name:a}).then(e=>{this.working=!1,this.dialog=!1,this.loadMarkovChains()}).catch(e=>{this.working=!1})},deleteMarkov:function(t){this.$root.$confirm("Delete",`This will remove ${t.name} from your Markov chains. Are you sure?`).then(a=>{this.loading=!0,this.axios.delete(this.$serverAddr+"/markovChains/"+t.id).then(e=>{this.loadMarkovChains()})})}}},s={};var f=c(u,v,m,!1,h,"8e58174a",null,null);function h(t){for(let a in s)this[a]=s[a]}var $=function(){return f.exports}();export{$ as default};
>>>>>>>> master:webadmin/fitcrackFE/dist/assets/markovView.0279fc88.js
