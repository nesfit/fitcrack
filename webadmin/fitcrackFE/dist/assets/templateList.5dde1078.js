<<<<<<<< HEAD:webadmin/fitcrackFE/dist/assets/templateList.b3533d4e.js
import{t as l}from"./fc_tile.217bb9a5.js";import{n as c}from"./index.d3f441ac.js";import"./vendor.6aa46313.js";var d=function(){var e=this,a=e.$createElement,t=e._self._c||a;return t("v-container",{staticClass:"max600"},[t("fc-tile",{staticClass:"ma-2",attrs:{title:"Job Templates",icon:e.$route.meta.icon}},[t("v-data-table",{attrs:{headers:e.headers,items:e.templates.items,loading:e.loading,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"Templates per page"}},scopedSlots:e._u([{key:"item.created",fn:function(n){var s=n.item;return[e._v(" "+e._s(e.$moment.utc(s.created).local().format("DD.MM.YYYY HH:mm"))+" ")]}},{key:"item.actions",fn:function(n){var s=n.item;return[t("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(o){var i=o.on;return[t("v-btn",e._g({attrs:{icon:""},on:{click:function(v){return e.deleteTemplate(s)}}},i),[t("v-icon",{attrs:{color:"error"}},[e._v(" mdi-delete-outline ")])],1)]}}],null,!0)},[t("span",[e._v("Delete")])])]}}])})],1)],1)},m=[];const u={components:{FcTile:l},data(){return{loading:!1,templates:[],headers:[{text:"Name",align:"start",value:"name"},{text:"Added",value:"created",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}]}},mounted(){this.load()},methods:{load(){this.loading=!0,this.axios.get(this.$serverAddr+"/template",{}).then(e=>{this.templates=e.data,this.loading=!1})},deleteTemplate({id:e}){this.loading=!0,this.axios.delete(this.$serverAddr+"/template/"+e).then(a=>{this.load()})}}},r={};var _=c(u,d,m,!1,p,"bc6568c6",null,null);function p(e){for(let a in r)this[a]=r[a]}var x=function(){return _.exports}();export{x as default};
========
import{t as l}from"./fc_tile.83a0e997.js";import{n as c}from"./index.c11e1020.js";import"./vendor.6aa46313.js";var m=function(){var e=this,a=e.$createElement,t=e._self._c||a;return t("v-container",{staticClass:"max600"},[t("fc-tile",{staticClass:"ma-2",attrs:{title:"Job Templates",icon:e.$route.meta.icon}},[t("v-data-table",{attrs:{headers:e.headers,items:e.templates.items,loading:e.loading,"footer-props":{itemsPerPageOptions:[10,25,50],itemsPerPageText:"Templates per page"}},scopedSlots:e._u([{key:"item.created",fn:function(n){var s=n.item;return[e._v(" "+e._s(e.$moment.utc(s.created).local().format("DD.MM.YYYY HH:mm"))+" ")]}},{key:"item.actions",fn:function(n){var s=n.item;return[t("v-tooltip",{attrs:{top:""},scopedSlots:e._u([{key:"activator",fn:function(o){var i=o.on;return[t("v-btn",e._g({attrs:{icon:""},on:{click:function(v){return e.deleteTemplate(s)}}},i),[t("v-icon",{attrs:{color:"error"}},[e._v(" mdi-delete-outline ")])],1)]}}],null,!0)},[t("span",[e._v("Delete")])])]}}])})],1)],1)},d=[];const u={components:{FcTile:l},data(){return{loading:!1,templates:[],headers:[{text:"Name",align:"start",value:"name"},{text:"Added",value:"created",align:"end"},{text:"Actions",value:"actions",align:"end",sortable:!1}]}},mounted(){this.load()},methods:{load(){this.loading=!0,this.axios.get(this.$serverAddr+"/template",{}).then(e=>{this.templates=e.data,this.loading=!1})},deleteTemplate({id:e}){this.loading=!0,this.axios.delete(this.$serverAddr+"/template/"+e).then(a=>{this.load()})}}},r={};var _=c(u,m,d,!1,p,"bc6568c6",null,null);function p(e){for(let a in r)this[a]=r[a]}var x=function(){return _.exports}();export{x as default};
>>>>>>>> master:webadmin/fitcrackFE/dist/assets/templateList.5dde1078.js
