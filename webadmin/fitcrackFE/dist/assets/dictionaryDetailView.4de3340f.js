import{f as n}from"./numberFormat.773889d3.js";import{f as s}from"./fc_textarea.fea06619.js";import{t as r}from"./fc_tile.c5ab6ff7.js";import{n as o}from"./index.a15c1e9d.js";import"./vue-infinite-loading.33b09e95.js";import"./vendor.6aa46313.js";var l=function(){var t=this,e=t.$createElement,i=t._self._c||e;return i("div",[t.info!=null?i("v-breadcrumbs",{attrs:{items:[{text:"Dictionaries",to:{name:"dictionaries"},exact:!0},{text:t.info.name}],divider:"/"}}):t._e(),i("v-container",[i("fc-tile",{staticClass:"mx-2 dictContentContainer mb-4",attrs:{title:"Dictionary",loading:t.info==null}},[t.info!=null?i("v-list",[i("v-list-item",[i("v-list-item-action",[t._v(" Name: ")]),i("v-list-item-content",[i("v-list-item-title",{staticClass:"text-right"},[t._v(" "+t._s(t.info.name)+" ")])],1)],1),i("v-divider"),i("v-list-item",[i("v-list-item-action",[t._v(" Keyspace: ")]),i("v-list-item-content",[i("v-list-item-title",{staticClass:"text-right"},[t._v(" "+t._s(t.fmt(t.info.keyspace))+" ")])],1)],1),i("v-divider"),i("v-list-item",[i("v-list-item-action",[t._v(" Words in HEX: ")]),i("v-list-item-content",[i("v-list-item-title",{staticClass:"text-right"},[t._v(" "+t._s(t.info.hex_dict?"Yes":"No")+" ")])],1)],1),i("v-divider"),i("v-list-item",[i("v-list-item-action",[t._v(" Added: ")]),i("v-list-item-content",[i("v-list-item-title",{staticClass:"text-right"},[t._v(" "+t._s(t.$moment.utc(t.info.time).local().format("DD.MM.YYYY HH:mm"))+" ")])],1)],1)],1):t._e(),i("v-divider"),t.info!=null?i("div",{staticClass:"dictContent"},[i("fc-textarea",{attrs:{"search-enable":!0,readonly:!0,url:this.$serverAddr+"/dictionary/"+this.$route.params.id+"/data","max-height":"500"}})],1):t._e()],1)],1)],1)},c=[];const m={name:"DictionaryView",components:{"fc-tile":r,"fc-textarea":s},data:function(){return{info:null}},mounted:function(){this.loadData()},methods:{fmt:n,loadData:function(t){this.axios.get(this.$serverAddr+"/dictionary/"+this.$route.params.id).then(e=>{this.info=e.data})}}},a={};var v=o(m,l,c,!1,d,"6fc1f071",null,null);function d(t){for(let e in a)this[e]=a[e]}var y=function(){return v.exports}();export{y as default};
