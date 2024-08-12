import{f as r}from"./numberFormat.773889d3.js";import{i,b as o}from"./index.09a07ba8.js";import{t as d}from"./fc_tile.6b33d265.js";import{n as c}from"./index.9908f285.js";import"./index.a9d6c6c2.js";import"./vendor.6aa46313.js";var l=function(){var t=this,a=t.$createElement,e=t._self._c||a;return e("div",[t.data!=null?e("v-breadcrumbs",{attrs:{items:[{text:"Charsets",to:{name:"charsets"},exact:!0},{text:t.data.name}],divider:"/"}}):t._e(),e("v-container",[e("v-row",{attrs:{justify:"center"}},[e("fc-tile",{staticClass:"mx-2 dictContentContainer mb-4",attrs:{title:"Charsets",loading:t.data==null}},[t.data!=null?e("v-list",[e("v-list-item",[e("v-list-item-action",[t._v(" Name: ")]),e("v-list-item-content",[e("v-list-item-title",{staticClass:"text-right"},[t._v(" "+t._s(t.data.name)+" ")])],1)],1),e("v-divider"),e("v-list-item",[e("v-list-item-action",[t._v(" Keyspace: ")]),e("v-list-item-content",[e("v-list-item-title",{staticClass:"text-right"},[t._v(" "+t._s(t.fmt(t.data.keyspace))+" ")])],1)],1),e("v-divider"),e("v-list-item",[e("v-list-item-action",[t._v(" Added: ")]),e("v-list-item-content",[e("v-list-item-title",{staticClass:"text-right"},[t._v(" "+t._s(t.$moment.utc(t.data.time).local().format("DD.MM.YYYY HH:mm"))+" ")])],1)],1)],1):t._e(),e("v-row",{staticClass:"mx-2 mb-2 align-center"},[e("v-col",[e("v-combobox",{attrs:{items:t.encodings,label:"View as",outlined:"",dense:"","hide-details":""},model:{value:t.as,callback:function(n){t.as=n},expression:"as"}})],1),e("v-col",[e("v-btn",{attrs:{color:"primary",href:t.$serverAddr+"/charset/"+t.data.id+"/download",target:"_blank"}},[t._v(" Download "),e("v-icon",{attrs:{right:""}},[t._v(" mdi-download ")])],1)],1)],1),e("v-divider"),t.data!=null?e("div",{staticClass:"dictContent pa-2"},[e("code",{staticClass:"code elevation-0"},[t._v(t._s(t.repr))])]):t._e()],1)],1)],1)],1)},m=[];const v={name:"CharsetDetailView",components:{"fc-tile":d},data:function(){return{data:null,as:"hex",encodings:["hex","utf-8","cp1250","cp1251","cp1252","iso-8859-1","iso-8859-2","iso-8859-5","iso-8859-15","koi8-r","koi8-u"]}},computed:{repr(){return i.encodingExists(this.as)?i.decode(o.Buffer.from(this.data.data,"base64"),this.as):(this.$error("Unrecognized encoding!"),"<unrecognized encoding selected>")}},mounted:function(){this.loadData()},methods:{fmt:r,loadData:function(t){this.axios.get(this.$serverAddr+"/charset/"+this.$route.params.id).then(a=>{this.data=a.data,this.as=this.encodings.find(e=>this.data.name.search(new RegExp(e,"i"))>=0)||"hex"})}}},s={};var _=c(v,l,m,!1,u,"9f361054",null,null);function u(t){for(let a in s)this[a]=s[a]}var C=function(){return _.exports}();export{C as default};
