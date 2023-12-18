import{n as i}from"./index.e9edaf92.js";var d=function(){var t=this,r=t.$createElement,e=t._self._c||r;return e("v-container",[t.noToolbar?t._e():e("v-toolbar",{attrs:{flat:""}},[e("v-toolbar-title",{staticClass:"mr-4"},[t._v("Hashes")]),e("v-spacer"),e("v-btn-toggle",{attrs:{dense:""},on:{change:t.loadHashes},model:{value:t.crackStatus,callback:function(a){t.crackStatus=a},expression:"crackStatus"}},[e("v-btn",{attrs:{value:"All",text:""}},[t._v("All")]),e("v-btn",{attrs:{value:"Cracked",text:""}},[t._v("Cracked")]),e("v-btn",{attrs:{value:"Uncracked",text:""}},[t._v("Uncracked")])],1),e("v-tooltip",{attrs:{bottom:""},scopedSlots:t._u([{key:"activator",fn:function(a){var s=a.on;return[e("v-btn",t._g({staticClass:"ml-4",attrs:{color:"primary",text:"",small:"",href:t.downloadLink}},s),[e("v-icon",{attrs:{left:""}},[t._v("mdi-download")]),t._v(" Download ")],1)]}}],null,!1,149201135)},[e("span",[t._v("Export current view. Respects set filters and ordering.")])]),t.linkBack?e("v-btn",{attrs:{color:"primary",text:"",small:"",to:{name:"hashlistDetail",params:{id:t.id}}}},[e("v-icon",{attrs:{left:""}},[t._v("mdi-open-in-new")]),t._v(" Open ")],1):t._e()],1),e("v-data-table",{ref:"table",attrs:{headers:t.headers,items:t.hashes,options:t.dataTableOptions,"server-items-length":t.totalItems,loading:t.loading,"footer-props":{itemsPerPageOptions:[25,50,100],itemsPerPageText:"Hashes per page"}},on:{"update:options":function(a){t.dataTableOptions=a}},scopedSlots:t._u([{key:"item.password",fn:function(a){var s=a.item;return[s.password?e("span",[t._v(t._s(s.password))]):e("span",{staticClass:"red--text"},[t._v("\u2013")])]}},{key:"item.hashText",fn:function(a){var s=a.item;return[e("div",{staticClass:"d-flex align-center mw-50"},[e("v-dialog",{attrs:{"max-width":"600"},scopedSlots:t._u([{key:"activator",fn:function(l){var o=l.on;return[e("span",t._g({staticClass:"text-truncate d-inline-block cursor-pointer"},o),[t._v(" "+t._s(t.truncateHashIfNeeded(s.hashText))+" ")])]}}],null,!0)},[e("v-card",[e("v-card-title",[t._v(" Full hash ")]),e("v-card-text",[t._v(" "+t._s(s.hashText)+" ")])],1)],1)],1)]}}])})],1)},c=[];const u={props:{id:Number,noToolbar:Boolean,linkBack:Boolean},data(){return{hashes:[],interval:null,crackStatus:"All",totalItems:0,dataTableOptions:{},loading:!0,headers:[{text:"Hash",value:"hashText",align:"start",sortable:!1},{text:"Password",value:"password",align:"end",sortable:!1},{text:"Completed",value:"time_cracked",align:"end",sortable:!1}]}},computed:{cracked(){switch(this.crackStatus){case"All":return null;case"Cracked":return!0;case"Uncracked":return!1}},downloadLink(){let t="?";return this.cracked!==null&&(t+=`cracked=${this.cracked}`),`${this.$serverAddr}/hashlist/${this.id}/download${t==="?"?"":t}`}},mounted:function(){this.loadHashes()},watch:{dataTableOptions:{handler:function(t){this.loadHashes()},deep:!0}},methods:{loadHashes(){this.loading=!0,this.axios.get(this.$serverAddr+"/hashlist/"+this.id+"/details",{params:{page:this.dataTableOptions.page,per_page:this.dataTableOptions.itemsPerPage,cracked:this.cracked,descending:this.dataTableOptions.sortDesc?this.dataTableOptions.sortDesc[0]:!1}}).then(t=>{this.hashes=t.data.items,this.totalItems=t.data.total,this.loading=!1})},truncateHashIfNeeded(t){return t.length>80?t.substr(0,70)+"...":t}}},n={};var h=i(u,d,c,!1,v,null,null,null);function v(t){for(let r in n)this[r]=n[r]}var m=function(){return h.exports}();export{m as H};
