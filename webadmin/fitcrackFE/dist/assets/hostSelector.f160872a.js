<<<<<<<< HEAD:webadmin/fitcrackFE/dist/assets/hostSelector.1b477e48.js
import"./index.09a07ba8.js";import{s as l}from"./selectorMixin.a647ee87.js";import{n as m}from"./index.d3f441ac.js";var c=function(){var e=this,a=e.$createElement,n=e._self._c||a;return n("v-data-table",{attrs:{headers:e.headers,items:e.items,"items-per-page":e.itemsPerPage,"footer-props":e.footerProps,loading:e.loading,"item-key":"id","show-select":"","single-select":!e.selectAll,"disable-pagination":"","hide-default-footer":""},on:{input:e.updateSelected},scopedSlots:e._u([{key:"item.domain_name",fn:function(s){var t=s.item;return[n("router-link",{staticClass:"middle",attrs:{to:{name:"hostDetail",params:{id:t.id}}}},[e._v(" "+e._s(t.domain_name+" ("+t.user.name+")")+" "),n("v-icon",{attrs:{small:"",color:"primary"}},[e._v(" mdi-open-in-new ")])],1)]}},{key:"item.jobs",fn:function(s){var t=s.item;return[e._v(" "+e._s(t.jobs.map(function(i){return i.status===10?1:0}).reduce(function(i,o){return i+o},0))+" ")]}},{key:"item.p_model",fn:function(s){var t=s.item;return[n("span",{staticClass:"oneline"},[e._v(e._s(t.p_model.replace(/(?:\(R\)|\(TM\)|Intel|AMD)/g,"")))])]}},{key:"item.last_active",fn:function(s){var t=s.item;return[t.last_active.seconds_delta>61?n("span",[e._v(e._s(e.parseTimeDelta(t.last_active.last_seen)))]):n("v-icon",{attrs:{color:"success"}},[e._v(" mdi-power ")])]}}]),model:{value:e.selected,callback:function(s){e.selected=s},expression:"selected"}})},d=[];const u={name:"HostSelector",mixins:[l],props:{autoRefresh:Boolean},data(){return{loading:!1,headers:[{text:"Name",align:"start",value:"domain_name"},{text:"IP address",value:"ip_address",align:"end",sortable:!1},{text:"OS",value:"os_name",align:"end",sortable:!1},{text:"Processor",value:"p_model",align:"end",width:"200",sortable:!1},{text:"Active jobs",value:"jobs",align:"center",sortable:!1},{text:"Online",value:"last_active",align:"end",sortable:!1}]}},mounted(){this.interval=setInterval(()=>{this.autoRefresh&&this.getData(!0)},2e3)},beforeDestroy(){clearInterval(this.interval)},methods:{getData(e=!1){e||(this.loading=!0),this.axios.get(this.$serverAddr+"/hosts",{params:{all:!0}}).then(a=>{this.items=a.data.items,e||(this.selected=this.items),this.loading=!1})},parseTimeDelta:function(e){return e!=null?this.$moment.utc(e).fromNow():"Unknown"}}},r={};var _=m(u,c,d,!1,v,"09960fd8",null,null);function v(e){for(let a in r)this[a]=r[a]}var g=function(){return _.exports}();export{g as h};
========
import"./index.09a07ba8.js";import{s as l}from"./selectorMixin.fb99fd96.js";import{n as m}from"./index.c11e1020.js";var c=function(){var e=this,a=e.$createElement,n=e._self._c||a;return n("v-data-table",{attrs:{headers:e.headers,items:e.items,"items-per-page":e.itemsPerPage,"footer-props":e.footerProps,loading:e.loading,"item-key":"id","show-select":"","single-select":!e.selectAll,"disable-pagination":"","hide-default-footer":""},on:{input:e.updateSelected},scopedSlots:e._u([{key:"item.domain_name",fn:function(s){var t=s.item;return[n("router-link",{staticClass:"middle",attrs:{to:{name:"hostDetail",params:{id:t.id}}}},[e._v(" "+e._s(t.domain_name+" ("+t.user.name+")")+" "),n("v-icon",{attrs:{small:"",color:"primary"}},[e._v(" mdi-open-in-new ")])],1)]}},{key:"item.jobs",fn:function(s){var t=s.item;return[e._v(" "+e._s(t.jobs.map(function(i){return i.status===10?1:0}).reduce(function(i,o){return i+o},0))+" ")]}},{key:"item.p_model",fn:function(s){var t=s.item;return[n("span",{staticClass:"oneline"},[e._v(e._s(t.p_model.replace(/(?:\(R\)|\(TM\)|Intel|AMD)/g,"")))])]}},{key:"item.last_active",fn:function(s){var t=s.item;return[t.last_active.seconds_delta>61?n("span",[e._v(e._s(e.parseTimeDelta(t.last_active.last_seen)))]):n("v-icon",{attrs:{color:"success"}},[e._v(" mdi-power ")])]}}]),model:{value:e.selected,callback:function(s){e.selected=s},expression:"selected"}})},d=[];const u={name:"HostSelector",mixins:[l],props:{autoRefresh:Boolean},data(){return{loading:!1,headers:[{text:"Name",align:"start",value:"domain_name"},{text:"IP address",value:"ip_address",align:"end",sortable:!1},{text:"OS",value:"os_name",align:"end",sortable:!1},{text:"Processor",value:"p_model",align:"end",width:"200",sortable:!1},{text:"Active jobs",value:"jobs",align:"center",sortable:!1},{text:"Online",value:"last_active",align:"end",sortable:!1}]}},mounted(){this.interval=setInterval(()=>{this.autoRefresh&&this.getData(!0)},2e3)},beforeDestroy(){clearInterval(this.interval)},methods:{getData(e=!1){e||(this.loading=!0),this.axios.get(this.$serverAddr+"/hosts",{params:{all:!0}}).then(a=>{this.items=a.data.items,e||(this.selected=this.items),this.loading=!1})},parseTimeDelta:function(e){return e!=null?this.$moment.utc(e).fromNow():"Unknown"}}},r={};var _=m(u,c,d,!1,f,"09960fd8",null,null);function f(e){for(let a in r)this[a]=r[a]}var g=function(){return _.exports}();export{g as h};
>>>>>>>> master:webadmin/fitcrackFE/dist/assets/hostSelector.f160872a.js
