<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.1">
  <compound kind="class">
    <name>Hurricane::Contact::AnchorHook</name>
    <filename>classHurricane_1_1Contact_1_1AnchorHook.html</filename>
    <base>Hurricane::Hook</base>
  </compound>
  <compound kind="class">
    <name>Hurricane::BasicLayer</name>
    <filename>classHurricane_1_1BasicLayer.html</filename>
    <base>Hurricane::Layer</base>
    <class kind="class">Hurricane::BasicLayer::Material</class>
    <member kind="function">
      <type>const Material &amp;</type>
      <name>getMaterial</name>
      <anchorfile>classHurricane_1_1BasicLayer.html</anchorfile>
      <anchor>aeb7fd37db4ecf8e56e1992d6350fac58</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getRealName</name>
      <anchorfile>classHurricane_1_1BasicLayer.html</anchorfile>
      <anchor>aaae9fcd776eb83407b4566614eafc41f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setBlockageLayer</name>
      <anchorfile>classHurricane_1_1BasicLayer.html</anchorfile>
      <anchor>a766c6dc1120de2066b15411861f5d4f8</anchor>
      <arglist>(BasicLayer *layer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRealName</name>
      <anchorfile>classHurricane_1_1BasicLayer.html</anchorfile>
      <anchor>aa5aa1e1079c14d7e9c05799d14e726af</anchor>
      <arglist>(const char *realName)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static BasicLayer *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1BasicLayer.html</anchorfile>
      <anchor>adff716f670e03c59ff970cd4fee298b6</anchor>
      <arglist>(Technology *technology, const Name &amp;name, const Material &amp;material, uint32_t gds2Layer=0, uint32_t gds2Datatype=0, const DbU::Unit &amp;minimalSize=0, const DbU::Unit &amp;minimalSpacing=0)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Component::BodyHook</name>
    <filename>classHurricane_1_1Component_1_1BodyHook.html</filename>
    <base>Hurricane::Hook</base>
  </compound>
  <compound kind="class">
    <name>Hurricane::Box</name>
    <filename>classHurricane_1_1Box.html</filename>
    <member kind="function">
      <type></type>
      <name>Box</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a445dd24bf83759bb47fc483fc7da024f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Box</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>af53adb323e9e89eef4e96da9efc33fe9</anchor>
      <arglist>(const DbU::Unit &amp;x, const DbU::Unit &amp;y)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Box</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a2f2aa57fa9486b508fca2a060648d04a</anchor>
      <arglist>(const Point &amp;point)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Box</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a101cd5a10d6cf229ccedccbb5417ed55</anchor>
      <arglist>(const DbU::Unit &amp;x1, const DbU::Unit &amp;y1, const DbU::Unit &amp;x2, const DbU::Unit &amp;y2)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Box</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a47f434b4dbda6af14a354722f66a47da</anchor>
      <arglist>(const Point &amp;point1, const Point &amp;point2)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Box</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>af9a7605270bf1ebb38723fba5b9d9236</anchor>
      <arglist>(const Box &amp;box)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a01abf59b3d3e99e694a7d4789f1bb978</anchor>
      <arglist>(const Box &amp;box)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a2a363ad0fdfda5a2f56b1b62a8665703</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a77a0e8c424c246973c455ce8e3ada8fb</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getXMin</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ad5122ef7dda8a58c1dacddb57cd4ccfb</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getYMin</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a542c383466845aeca0e32f51b77c7439</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getXMax</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a77b9db757080544fcede3e670cee8c5c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getYMax</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a06e1a86a06dacfca6d3403c16affc7e8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getXCenter</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a95f35cd33966aad61bb43662306ccf98</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getYCenter</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a659820726e3862a70158b6f5b7644da0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Point</type>
      <name>getCenter</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ac18c8725989166d1b101de29531e4f6e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getWidth</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ae2cc9cf0b17e6443a88b475bbd36e4c9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHalfWidth</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a8aa689ad799e4c78bfefb0328e7d9081</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHeight</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a7b15b9488d49da1fc666c0383fb213ab</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHalfHeight</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a5c94554a78398c4a5c7dedd024926abb</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Box</type>
      <name>getUnion</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a2670058f109dfae32d284db249e533bc</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>Box</type>
      <name>getIntersection</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a610f9c63bc5636ef304f4768215ffb12</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isEmpty</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>af8b269603b5c173891a484214ca50266</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isFlat</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a0df4d580a3dc1eb23d839c0f53cdee8b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isPonctual</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a3d073c5bc3d0ea1b4f21937e36be001f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>contains</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ae18dd30ffbf0b75714ece480f21e2898</anchor>
      <arglist>(const DbU::Unit &amp;x, const DbU::Unit &amp;y) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>contains</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a19ad23904fbfe2afb3683affeb2cac7e</anchor>
      <arglist>(const Point &amp;point) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>contains</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ac567c569f23643e58867afee80f6920a</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>intersect</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ae76b57bf6399b29021813da8d3f306ec</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConstrainedBy</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a70d832443d97cb40ec7cb4f0f959a977</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>makeEmpty</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a0717b1b105f65f8284c9b4e36df3a766</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>inflate</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a90207e7ca8044a6afc72674cc6ae366e</anchor>
      <arglist>(const DbU::Unit &amp;d)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>inflate</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a6b97ea9d54fbf4dae52459073cdf4b5f</anchor>
      <arglist>(const DbU::Unit &amp;dx, const DbU::Unit &amp;dy)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>inflate</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>afd1baf9f272878a87c2525f0fa2eab71</anchor>
      <arglist>(const DbU::Unit &amp;dxMin, const DbU::Unit &amp;dyMin, const DbU::Unit &amp;dxMax, const DbU::Unit &amp;dyMax)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ab77fe56f9350f06cc872bbb4f83835da</anchor>
      <arglist>(const DbU::Unit &amp;x, const DbU::Unit &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>af1f7dfe8984c2d26fbca78b21358ee2b</anchor>
      <arglist>(const Point &amp;point)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>ad97e73e91dd36404eb0dde9d44ff2fd7</anchor>
      <arglist>(const DbU::Unit &amp;x1, const DbU::Unit &amp;y1, const DbU::Unit &amp;x2, const DbU::Unit &amp;y2)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>a0bdfa52a3f5f6639680ba7dbc52c21d7</anchor>
      <arglist>(const Box &amp;box)</arglist>
    </member>
    <member kind="function">
      <type>Box &amp;</type>
      <name>translate</name>
      <anchorfile>classHurricane_1_1Box.html</anchorfile>
      <anchor>aa689be4b37c83412f3dc95fc23c82156</anchor>
      <arglist>(const DbU::Unit &amp;dx, const DbU::Unit &amp;dy)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Box.html" title="Introduction">secBoxIntro</docanchor>
    <docanchor file="classHurricane_1_1Box.html" title="Remark on Modifiers">secBoxModifierRemark</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Cell</name>
    <filename>classHurricane_1_1Cell.html</filename>
    <base>Hurricane::Entity</base>
    <member kind="function">
      <type>Library *</type>
      <name>getLibrary</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>aa84b97773160a28d3dd69df1e261eecf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a01cd4bba972d484496fd297648b8fa0c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Instance *</type>
      <name>getInstance</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>abaf178b24734de37cf0ac31918c096ac</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>Instances</type>
      <name>getInstances</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>aa85b3992431b672827167c5d9cb622f2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Instances</type>
      <name>getInstancesUnder</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a3af933175d318b205d94adaf92ba0499</anchor>
      <arglist>(const Box &amp;area, DbU::Unit threshold=0) const</arglist>
    </member>
    <member kind="function">
      <type>Instances</type>
      <name>getSlaveInstances</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a7e51bee5db73dd44f788e591a5c175c8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Net *</type>
      <name>getNet</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a70f06abd224895aeeeeb042365dbf48a</anchor>
      <arglist>(const Name &amp;name, bool useAlias=true) const</arglist>
    </member>
    <member kind="function">
      <type>Nets</type>
      <name>getNets</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a8b4728abe83e9ec21d7bee1154218279</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Nets</type>
      <name>getGlobalNets</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a1512722d821edc18ff38e673862cd108</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Nets</type>
      <name>getExternalNets</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>aa80f3345db8c1395fa04a50737208793</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Nets</type>
      <name>getInternalNets</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a0da980d28ad60334da94a3966338f873</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Nets</type>
      <name>getClockNets</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a306f28990f9fd4ccee0e9e8ebecf98fc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Nets</type>
      <name>getSupplyNets</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>ac51c8f16de7a4af86feead9f1aecf494</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Slice *</type>
      <name>getSlice</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>ac438b5b6b8dbcd868d6bf0deeb469444</anchor>
      <arglist>(const Layer *layer) const</arglist>
    </member>
    <member kind="function">
      <type>Slices</type>
      <name>getSlices</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>aba933a81e3cacfc05b7bd1660e2a933a</anchor>
      <arglist>(const Layer::Mask &amp;mask=~0) const</arglist>
    </member>
    <member kind="function">
      <type>Rubbers</type>
      <name>getRubbers</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a56395a189898d5ae2a869d5a5d5dfdbe</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Rubbers</type>
      <name>getRubbersUnder</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a58c6e24401d15f375547ad95b5c2c27c</anchor>
      <arglist>(const Box &amp;area) const</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getComponents</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a14cb1b1f27e75d4af5b34a9a5956d818</anchor>
      <arglist>(const Layer::Mask &amp;mask=~0) const</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getComponentsUnder</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a0a3c54d755ab36fe74bd032dfd43b53a</anchor>
      <arglist>(const Box &amp;area, const Layer::Mask &amp;mask=~0) const</arglist>
    </member>
    <member kind="function">
      <type>Occurrences</type>
      <name>getOccurrences</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>ab5bbab0a59106855d61deb94805e6115</anchor>
      <arglist>(unsigned searchDepth=std::numeric_limits&lt; unsigned int &gt;::max()) const</arglist>
    </member>
    <member kind="function">
      <type>Occurrences</type>
      <name>getOccurrencesUnder</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>aee27efc0497992f811c9812ffa272a5a</anchor>
      <arglist>(const Box &amp;area, unsigned searchDepth=std::numeric_limits&lt; unsigned int &gt;::max(), DbU::Unit threshold=0) const</arglist>
    </member>
    <member kind="function">
      <type>Occurrences</type>
      <name>getTerminalInstanceOccurrences</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a30b71d9a35ff4e0b59b98ef515f26fc0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Occurrences</type>
      <name>getTerminalNetlistInstanceOccurrences</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a6f559f7dab6e4afc0b60eba064c5e474</anchor>
      <arglist>(const Instance *topInstance=NULL) const</arglist>
    </member>
    <member kind="function">
      <type>Occurrences</type>
      <name>getNonTerminalNetlistInstanceOccurrences</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a9e7a0536ec1efb23be2764068a85b6a7</anchor>
      <arglist>(const Instance *topInstance=NULL) const</arglist>
    </member>
    <member kind="function">
      <type>const Box &amp;</type>
      <name>getAbutmentBox</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a142360ca7b3c1c637894f5b9a2cac069</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isCalledBy</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a239354e1b4ad9b751abf5a064e43b0e6</anchor>
      <arglist>(Cell *cell) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isTerminal</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>aac4e9218b7806f3a0f2d5a55f00abd69</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isTerminalNetlist</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a6fe2b5a80d4b344733416b25ea559497</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isUnique</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a6c2f2fd9f6f6e0578937a90c0c37a507</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isUniquified</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a86c21867e9ce896eae72fd2999ce8a2d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isUniquifyMaster</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a0220dbbbe730e6874f7620135e9c10f6</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>ad2c9face922062664110c66ee205eab2</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setAbutmentBox</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>ab1949e2b708f0bd2d215ab90cfe864e0</anchor>
      <arglist>(const Box &amp;abutmentBox)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setTerminalNetlist</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a47ce34631bb9f6862caa13e5b25a4d8f</anchor>
      <arglist>(bool state)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>materialize</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>affefc597317063857f4904d4b16d5d4f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unmaterialize</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a40c9ba4e3fc76b0c4bc58af8dcaddf53</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>a092f53c7f517ecc70d9ba375296c5d5b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>uniquify</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>aa113c121813342b6304f3e7fddbc8565</anchor>
      <arglist>(unsigned int depth=std::numeric_limits&lt; unsigned int &gt;::max())</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Cell *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Cell.html</anchorfile>
      <anchor>ad803afb3e52bea3bf3d520e353b162e0</anchor>
      <arglist>(Library *library, const Name &amp;name)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Cell.html" title="Layout vs. Netlist Cell Hierarchy">secCellHierarchy</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Collection</name>
    <filename>classHurricane_1_1Collection.html</filename>
    <templarg></templarg>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Collection</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aafcf8e05658e245b2a762baa7a59f281</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Collection&lt; Type &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>ac75b91d3952b36e14f21174958523924</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Type &gt; *</type>
      <name>getLocator</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a48fd1a0a2b6d2530a87e22ba65aa3152</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual unsigned</type>
      <name>getSize</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a1292aabe88c9aadfdfe21dabddb62c19</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Type</type>
      <name>getFirst</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a846a042646e02a0f77d2ce0f6190288a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; Type &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aa32ea7249d57ee05e3c71dcde8106832</anchor>
      <arglist>(const Filter&lt; Type &gt; &amp;filter) const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a91d986e21395d4021d927e06f204ab6c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a673afd14782da82ad03a68366ae1f09b</anchor>
      <arglist>(const Filter&lt; SubType &gt; &amp;filter) const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Collection.html" title="Introduction">secCollectionIntro</docanchor>
    <docanchor file="classHurricane_1_1Collection.html" title="STL Iterator Support">secIterator</docanchor>
    <docanchor file="classHurricane_1_1Collection.html" title="The forEach Macro (obsoleted)">secForEachMacro</docanchor>
    <docanchor file="classHurricane_1_1Collection.html" title="The Generic getCollection">secGenericgetCollection</docanchor>
    <docanchor file="classHurricane_1_1Collection.html" title="Locators">secCollectionLocators</docanchor>
  </compound>
  <compound kind="class">
    <name>Collection&lt; Cell * &gt;</name>
    <filename>classHurricane_1_1Collection.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Collection</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aafcf8e05658e245b2a762baa7a59f281</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Collection&lt; Cell * &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>ac75b91d3952b36e14f21174958523924</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Cell * &gt; *</type>
      <name>getLocator</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a48fd1a0a2b6d2530a87e22ba65aa3152</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual unsigned</type>
      <name>getSize</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a1292aabe88c9aadfdfe21dabddb62c19</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getFirst</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a846a042646e02a0f77d2ce0f6190288a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; Cell * &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aa32ea7249d57ee05e3c71dcde8106832</anchor>
      <arglist>(const Filter&lt; Cell * &gt; &amp;filter) const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a91d986e21395d4021d927e06f204ab6c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a673afd14782da82ad03a68366ae1f09b</anchor>
      <arglist>(const Filter&lt; SubType &gt; &amp;filter) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Collection&lt; Element &gt;</name>
    <filename>classHurricane_1_1Collection.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Collection</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aafcf8e05658e245b2a762baa7a59f281</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Collection&lt; Element &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>ac75b91d3952b36e14f21174958523924</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Element &gt; *</type>
      <name>getLocator</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a48fd1a0a2b6d2530a87e22ba65aa3152</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual unsigned</type>
      <name>getSize</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a1292aabe88c9aadfdfe21dabddb62c19</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Element</type>
      <name>getFirst</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a846a042646e02a0f77d2ce0f6190288a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; Element &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aa32ea7249d57ee05e3c71dcde8106832</anchor>
      <arglist>(const Filter&lt; Element &gt; &amp;filter) const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a91d986e21395d4021d927e06f204ab6c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a673afd14782da82ad03a68366ae1f09b</anchor>
      <arglist>(const Filter&lt; SubType &gt; &amp;filter) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Collection&lt; SubType &gt;</name>
    <filename>classHurricane_1_1Collection.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Collection</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aafcf8e05658e245b2a762baa7a59f281</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Collection&lt; SubType &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>ac75b91d3952b36e14f21174958523924</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; SubType &gt; *</type>
      <name>getLocator</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a48fd1a0a2b6d2530a87e22ba65aa3152</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual unsigned</type>
      <name>getSize</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a1292aabe88c9aadfdfe21dabddb62c19</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>SubType</type>
      <name>getFirst</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a846a042646e02a0f77d2ce0f6190288a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>aa32ea7249d57ee05e3c71dcde8106832</anchor>
      <arglist>(const Filter&lt; SubType &gt; &amp;filter) const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a91d986e21395d4021d927e06f204ab6c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>GenericCollection&lt; SubType &gt;</type>
      <name>getSubSet</name>
      <anchorfile>classHurricane_1_1Collection.html</anchorfile>
      <anchor>a673afd14782da82ad03a68366ae1f09b</anchor>
      <arglist>(const Filter&lt; SubType &gt; &amp;filter) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Entity::CompareById</name>
    <filename>classEntity_1_1CompareById.html</filename>
  </compound>
  <compound kind="class">
    <name>Hurricane::Component</name>
    <filename>classHurricane_1_1Component.html</filename>
    <base>Hurricane::Go</base>
    <class kind="class">Hurricane::Component::BodyHook</class>
    <member kind="typedef">
      <type>Go</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a3911e94f9d220eb809d349b1181034e3</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Net *</type>
      <name>getNet</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a1556ef77d6b89bfc17698d52ebde9791</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Rubber *</type>
      <name>getRubber</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>ab701515debfdf1bf4142d4d917aaab1d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getBodyHook</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a19f06d7cad163bab3b97a13f4736c9d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Hooks</type>
      <name>getHooks</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a1fc513b9465b2b8c22f41d56bf775594</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DbU::Unit</type>
      <name>getX</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a0f8299ed73705fd4fbf56589dcc7e074</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DbU::Unit</type>
      <name>getY</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a727da3f127c3a7a0a09468219f98c3e6</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Point</type>
      <name>getPosition</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>aa4e9a47c89fe701670ca34355195d519</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual const Layer *</type>
      <name>getLayer</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>ab451ef19059e6e5bbb77ae391d02a039</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Box</type>
      <name>getBoundingBox</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>aabb87b9ef71f71cea681a03a6213f616</anchor>
      <arglist>(const BasicLayer *) const =0</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getConnexComponents</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a4acf996c03b0b94a186fad653ba578a6</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getSlaveComponents</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>af6d6b7c6b3cb18754cfa02bc5fb1e754</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static ComponentFilter</type>
      <name>getIsUnderFilter</name>
      <anchorfile>classHurricane_1_1Component.html</anchorfile>
      <anchor>a8680f2756892366db8642bfcfd7ce097</anchor>
      <arglist>(const Box &amp;area)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Component.html" title="Introduction">secComponentIntro</docanchor>
    <docanchor file="classHurricane_1_1Component.html" title="Concept of Location">secComponentConceptOfLocation</docanchor>
    <docanchor file="classHurricane_1_1Component.html" title="Destruction">secComponentDestruction</docanchor>
    <docanchor file="classHurricane_1_1Component.html" title="Predefined filters">secComponentPredefinedFilters</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Contact</name>
    <filename>classHurricane_1_1Contact.html</filename>
    <base>Hurricane::Component</base>
    <class kind="class">Hurricane::Contact::AnchorHook</class>
    <member kind="typedef">
      <type>Component</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a422f15bba0561d8499c001fb8cbe6b67</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getAnchorHook</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a300306b006397377bc9a54ea783c1150</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Component *</type>
      <name>getAnchor</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>ab0b327b306bf7ebda634f59d8d0cfd8f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getDx</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a8a5c4475668b6c6730ed5265e5447553</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getDy</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>af674c59fcaf1f5214d54a558fe30e41a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getWidth</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a794ce7c3aa5ffe894c1231f7c5ac3c52</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHalfWidth</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a4a5136f4e8299435e50db7da28172ca1</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHeight</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a07a4ecc7ea2479e2d63f5f31d9325dde</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHalfHeight</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>aebd3ff8e1368617ab750b20ae9ffb59b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLayer</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>aec627634d5b6cfc5079a02b1b518b50e</anchor>
      <arglist>(const Layer *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setWidth</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a08d14ce6cdf3696e472f4a621b936afe</anchor>
      <arglist>(DbU::Unit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setHeight</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a6480b6a75cc098d3227f27080a2cb42b</anchor>
      <arglist>(DbU::Unit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSizes</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a1bded13596d448c6bb9c93271fffe5fd</anchor>
      <arglist>(DbU::Unit width, DbU::Unit height)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setX</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a5b2338675993259feabb641fd9a1996e</anchor>
      <arglist>(DbU::Unit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setY</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a232a49a5dd180e9ff8dfb2bd2a67f2cd</anchor>
      <arglist>(DbU::Unit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPosition</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>ae44d4d7655428705f13dca34c7167690</anchor>
      <arglist>(DbU::Unit x, DbU::Unit y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPosition</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>aedcc63fe54538939c03fe81a16b0bae0</anchor>
      <arglist>(const Point &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDx</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a82f29c6b48b0c5a51fe3c1678d71876c</anchor>
      <arglist>(DbU::Unit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDy</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>ac5dadc06ae38c1ff287f031864f58850</anchor>
      <arglist>(DbU::Unit)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setOffset</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a5c8cb75debcbe10aedc092e2089a975c</anchor>
      <arglist>(DbU::Unit dx, DbU::Unit dy)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Contact *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>ab66989c2dce4d398f1f7647aca50d983</anchor>
      <arglist>(Net *net, const Layer *layer, DbU::Unit x, DbU::Unit y, DbU::Unit width=0, DbU::Unit height=0)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Contact *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Contact.html</anchorfile>
      <anchor>a2e555edb8984b599c391f16db105c1f5</anchor>
      <arglist>(Component *anchor, const Layer *layer, DbU::Unit dx, DbU::Unit dy, DbU::Unit width=0, DbU::Unit height=0)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Contact.html" title="Introduction">secContactIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::ContactLayer</name>
    <filename>classHurricane_1_1ContactLayer.html</filename>
    <base>Hurricane::Layer</base>
    <member kind="function" static="yes">
      <type>static ContactLayer *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1ContactLayer.html</anchorfile>
      <anchor>ab5f2e78865e0311fcabf25e1aa94bf09</anchor>
      <arglist>(Technology *technology, const Name &amp;name, BasicLayer *metalLayer, BasicLayer *cutLayer, BasicLayer *activeLayer, BasicLayer *diffusionLayer, BasicLayer *wellLayer)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::DataBase</name>
    <filename>classHurricane_1_1DataBase.html</filename>
    <base>Hurricane::DBo</base>
    <member kind="function">
      <type>Technology *</type>
      <name>getTechnology</name>
      <anchorfile>classHurricane_1_1DataBase.html</anchorfile>
      <anchor>a144480c54b0f9fbda57622ad6767ab8a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Library *</type>
      <name>getRootLibrary</name>
      <anchorfile>classHurricane_1_1DataBase.html</anchorfile>
      <anchor>a4469391a3c5ae82caf090f1bdac4f29b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DataBase *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1DataBase.html</anchorfile>
      <anchor>af0210f9bb13faf06e12eb135eeea9b06</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DataBase *</type>
      <name>getDB</name>
      <anchorfile>classHurricane_1_1DataBase.html</anchorfile>
      <anchor>a53d0b9fcd06b73f3968c8f238f377a88</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::DBo</name>
    <filename>classHurricane_1_1DBo.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>destroy</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>a67febf5bf9c8b322674648688639728b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Property *</type>
      <name>getProperty</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>a599f61978df51d1d4c351f6cbd02488d</anchor>
      <arglist>(const Name &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>Properties</type>
      <name>getProperties</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>aec46894a10e83abb54c495dc4d90f2d3</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasProperty</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>a1563f094565030c77592ed82f9a9989b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>put</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>a8979674f11507cb4c7c5251b41ed72d5</anchor>
      <arglist>(Property *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>a7833a1f0b8c704930bdc00861e63cf5e</anchor>
      <arglist>(Property *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeProperty</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>ac35fbb8303b1a78db5ca0fc831fb6a0c</anchor>
      <arglist>(const Name &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clearProperties</name>
      <anchorfile>classHurricane_1_1DBo.html</anchorfile>
      <anchor>a3e02f3d665cb0b2120df2fdfe9c3df4f</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1DBo.html" title="Introduction">sDBoIntro</docanchor>
    <docanchor file="classHurricane_1_1DBo.html" title="Creation process">sDBoCreation</docanchor>
    <docanchor file="classHurricane_1_1DBo.html" title="Deletion process">sDBodestroy</docanchor>
    <docanchor file="classHurricane_1_1DBo.html" title="Extentions">sDBoExtentions</docanchor>
    <docanchor file="classHurricane_1_1DBo.html" title="Remark">sDBoRemark</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::DbU</name>
    <filename>classHurricane_1_1DbU.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>StringMode</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6af6a5b8d113a661fea65b2bcb8b25c4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Db</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6af6a5b8d113a661fea65b2bcb8b25c4a1b91af5faf467afcb73dec10bc54f233</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Grid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6af6a5b8d113a661fea65b2bcb8b25c4ac6b6574b2ef79ee4e44c6c00fe757c7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Symbolic</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6af6a5b8d113a661fea65b2bcb8b25c4a16f8df0900c42b001f0a91475a1b93f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>std::int64_t</type>
      <name>Unit</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a4fbfa3e8c89347af76c9628ea06c4146</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Db</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6af6a5b8d113a661fea65b2bcb8b25c4a1b91af5faf467afcb73dec10bc54f233</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Grid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6af6a5b8d113a661fea65b2bcb8b25c4ac6b6574b2ef79ee4e44c6c00fe757c7c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Symbolic</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6af6a5b8d113a661fea65b2bcb8b25c4a16f8df0900c42b001f0a91475a1b93f8</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>fromDb</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>aec69d65ec1651c2feea24c5931f4580b</anchor>
      <arglist>(Unit value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>fromGrid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a367e1d1b5ac1df076745550cba8a83c1</anchor>
      <arglist>(double value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>fromLambda</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a4b570755b19ea9ff0f2f258a221bd935</anchor>
      <arglist>(double value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>fromPhysical</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a11d4dbd9134a19bda35cbacde1cb2769</anchor>
      <arglist>(double value, UnitPower p)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned int</type>
      <name>getPrecision</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a6169efbdd9b3d54a0bd8467c8f957fda</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static unsigned int</type>
      <name>getMaximalPrecision</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a8756c9f0a32af5f601cd150e73b02c03</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>getResolution</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a120a60b09b344d01c583567a1e489d9e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setPrecision</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>ace9a8644e7e80dcaed2a8a95deeb1622</anchor>
      <arglist>(unsigned int precision, unsigned int flags=NoFlags)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setGridsPerLambda</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>ac93f9ba2a09105227e34bd05bcb1500c</anchor>
      <arglist>(double gridsPerLambda, unsigned int flags=NoFlags)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>getGridsPerLambda</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a9a0359adbfafc356326f5c6adf57ff04</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DbU::Unit</type>
      <name>getRealSnapGridStep</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a09e46fcca6aaca94851adfa196e10170</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DbU::Unit</type>
      <name>getOnRealSnapGrid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a8746e486f153aa37ee469c1604eba5c0</anchor>
      <arglist>(DbU::Unit u, SnapMode mode=Nearest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setRealSnapGridStep</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a202cc3aa3364c2224647a29dde047fae</anchor>
      <arglist>(DbU::Unit step)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DbU::Unit</type>
      <name>getSymbolicSnapGridStep</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a687a9134729b107c42fb7f69596c4c3b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DbU::Unit</type>
      <name>getOnSymbolicSnapGrid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>ad1b0c0f3680093cf5a63d901312c925d</anchor>
      <arglist>(DbU::Unit u, SnapMode mode=Nearest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setSymbolicSnapGridStep</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a9ccd423c8f268ef54770f4663e6c9304</anchor>
      <arglist>(DbU::Unit step)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DbU::Unit</type>
      <name>getOnCustomGrid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a87323d9038656dceabffc37d45de408a</anchor>
      <arglist>(DbU::Unit u, DbU::Unit step, SnapMode mode=Nearest)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DbU::Unit</type>
      <name>getOnPhysicalGrid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a9419025221579f4277475c65655be3dc</anchor>
      <arglist>(DbU::Unit u, SnapMode mode=Superior)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>toDb</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>aec07c6e7ae2a2a6f54e2a16b32c8bf26</anchor>
      <arglist>(Unit u)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>toGrid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a318d673386c9424e07c12efd598c730d</anchor>
      <arglist>(Unit u)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>toLambda</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a4923a9a443871282ad7d331be2a2a5d4</anchor>
      <arglist>(Unit u)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>toPhysical</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>ab901e9d5c12e878728178f113def6c45</anchor>
      <arglist>(Unit u, UnitPower p)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static string</type>
      <name>getValueString</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>adc9c1a06b4296dbddcf711077113f4bd</anchor>
      <arglist>(Unit u, int mode=SmartTruncate)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setStringMode</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a89ab8f8326c54113336086663ecf1d25</anchor>
      <arglist>(unsigned int mode, UnitPower p=Nano)</arglist>
    </member>
    <member kind="enumvalue">
      <name>Pico</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a3cf34ad82faf73a9b48dcb3a621d0557</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Nano</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a03e5923be5810db830626f2ca26319d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Micro</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7aa0481a3398a6cbb0a68a523146f0a7fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Milli</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7aac2973886c68f16ee68a192154ea65be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Unity</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7ac5c524bb7247124f3dce7d1dbdc7d2c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Kilo</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a7853e18601786b5c51a1bc9cfaf8bb74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>UnitPower</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Pico</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a3cf34ad82faf73a9b48dcb3a621d0557</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Nano</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a03e5923be5810db830626f2ca26319d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Micro</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7aa0481a3398a6cbb0a68a523146f0a7fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Milli</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7aac2973886c68f16ee68a192154ea65be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Unity</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7ac5c524bb7247124f3dce7d1dbdc7d2c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Kilo</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a7853e18601786b5c51a1bc9cfaf8bb74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Inferior</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a888eae532f84c3f19b024e1830ef8cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Superior</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a8ce92cf7ff7627c46baf85612f9ad847</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Nearest</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a65e6f47eb16779b8974a80d6145a2db5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>SnapMode</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Inferior</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a888eae532f84c3f19b024e1830ef8cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Superior</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a8ce92cf7ff7627c46baf85612f9ad847</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Nearest</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a65e6f47eb16779b8974a80d6145a2db5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Pico</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a3cf34ad82faf73a9b48dcb3a621d0557</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Nano</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a03e5923be5810db830626f2ca26319d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Micro</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7aa0481a3398a6cbb0a68a523146f0a7fb</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Milli</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7aac2973886c68f16ee68a192154ea65be</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Unity</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7ac5c524bb7247124f3dce7d1dbdc7d2c6</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Kilo</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a50b5785bf4d75026c4c112caec3040a7a7853e18601786b5c51a1bc9cfaf8bb74</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Inferior</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a888eae532f84c3f19b024e1830ef8cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Superior</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a8ce92cf7ff7627c46baf85612f9ad847</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>Nearest</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1082168d6f9956ebba22ab8bbec21637a65e6f47eb16779b8974a80d6145a2db5</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>db</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>acd77957381fb93fc4203bdca215e0b48</anchor>
      <arglist>(Unit value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>grid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a1d4bac6e3b68c8cd44b345de3b425753</anchor>
      <arglist>(double value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>lambda</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>aa1ba98acc939ff1c370c18544a5e0dce</anchor>
      <arglist>(double value)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Unit</type>
      <name>getDb</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>a4233772b1b3e68f3ec723c7509ea87ff</anchor>
      <arglist>(Unit u)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>getGrid</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>ad4485d0d7b5fd7ae87b32f165155c0a2</anchor>
      <arglist>(Unit u)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static double</type>
      <name>getLambda</name>
      <anchorfile>classHurricane_1_1DbU.html</anchorfile>
      <anchor>adea6b9a6e84243f70f3a5e2725b2c6d8</anchor>
      <arglist>(Unit u)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::DebugSession</name>
    <filename>classHurricane_1_1DebugSession.html</filename>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isTraced</name>
      <anchorfile>classHurricane_1_1DebugSession.html</anchorfile>
      <anchor>a0b008c0eb9a0337416465caf7431a81e</anchor>
      <arglist>(const void *symbol)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>addToTrace</name>
      <anchorfile>classHurricane_1_1DebugSession.html</anchorfile>
      <anchor>aa7ee4a8517bc4a99c37f0a9ae26b9620</anchor>
      <arglist>(const void *symbol)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>addToTrace</name>
      <anchorfile>classHurricane_1_1DebugSession.html</anchorfile>
      <anchor>ab7f9fbef88775e509ae7b127e524b100</anchor>
      <arglist>(const Cell *, const Name &amp;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>open</name>
      <anchorfile>classHurricane_1_1DebugSession.html</anchorfile>
      <anchor>ac12865b68d1acfd85cd48d4d44d9c4fc</anchor>
      <arglist>(int minLevel, int maxLevel)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>open</name>
      <anchorfile>classHurricane_1_1DebugSession.html</anchorfile>
      <anchor>a655f87fd8c8e20f287dea2a6d8fca556</anchor>
      <arglist>(const void *symbol, int minLevel, int maxLevel)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>close</name>
      <anchorfile>classHurricane_1_1DebugSession.html</anchorfile>
      <anchor>ac880eca99eeec60c669c0696f495ac42</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1DebugSession.html" title="Trace Levels">secTraceLevels</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Diagonal</name>
    <filename>classHurricane_1_1Diagonal.html</filename>
    <base>Hurricane::Component</base>
    <member kind="typedef">
      <type>Component</type>
      <name>Super</name>
      <anchorfile>classHurricane_1_1Diagonal.html</anchorfile>
      <anchor>aef5120a04b3b4db78b118e8c5daade90</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Diagonal *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Diagonal.html</anchorfile>
      <anchor>a83cd4332e78515e2c3745fd017e7441f</anchor>
      <arglist>(Net *, const Layer *, const Point &amp;source, const Point &amp;target, DbU::Unit width)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Diagonal.html" title="Introduction">secDiagonalIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::DiffusionLayer</name>
    <filename>classHurricane_1_1DiffusionLayer.html</filename>
    <base>Hurricane::Layer</base>
    <member kind="function" static="yes">
      <type>static DiffusionLayer *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1DiffusionLayer.html</anchorfile>
      <anchor>a91b5f8a20b005c20b4b9b9080250939e</anchor>
      <arglist>(Technology *technology, const Name &amp;name, BasicLayer *activeLayer, BasicLayer *diffusionLayer, BasicLayer *wellLayer)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Net::Direction</name>
    <filename>classHurricane_1_1Net_1_1Direction.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>Code</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DirIn</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a36971421023586a2b5b019f468d699ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DirOut</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a1135f8c6a05d3801c43684bc195f66f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DirUndefined</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a368b35a5f289879ad5c6862dfebc1b96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ConnTristate</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4afa0b4523129378e11f6e9bdc72fba627</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ConnWiredOr</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a03861307a54d5204f34c74365aa58f04</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNDEFINED</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4ad15ab42a0127de740e1c2c05841c153a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IN</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4aae6e926e7787f90824a4ee961e6ddac1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a2dac58452d767718df817bcf65906969</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INOUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4aa88aea57d992b95a08c24716b5265afd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TRISTATE</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a71a487a8129c354630d5e989d3994c98</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TRANSCV</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4ab8965ba57b68fc4b58c428fc4c8da397</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WOR_OUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a1cc48f96bc28740eb7f0d7ba7e2b237c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WOR_INOUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a2fbc95d7882aab3453d5549493763c3c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DirIn</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a36971421023586a2b5b019f468d699ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DirOut</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a1135f8c6a05d3801c43684bc195f66f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DirUndefined</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a368b35a5f289879ad5c6862dfebc1b96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ConnTristate</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4afa0b4523129378e11f6e9bdc72fba627</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>ConnWiredOr</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a03861307a54d5204f34c74365aa58f04</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNDEFINED</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4ad15ab42a0127de740e1c2c05841c153a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>IN</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4aae6e926e7787f90824a4ee961e6ddac1</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>OUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a2dac58452d767718df817bcf65906969</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>INOUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4aa88aea57d992b95a08c24716b5265afd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TRISTATE</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a71a487a8129c354630d5e989d3994c98</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>TRANSCV</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4ab8965ba57b68fc4b58c428fc4c8da397</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WOR_OUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a1cc48f96bc28740eb7f0d7ba7e2b237c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>WOR_INOUT</name>
      <anchorfile>classHurricane_1_1Net_1_1Direction.html</anchorfile>
      <anchor>a5b34d7c3ac52628861af3a46f781fae4a2fbc95d7882aab3453d5549493763c3c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Entity</name>
    <filename>classHurricane_1_1Entity.html</filename>
    <base>Hurricane::DBo</base>
    <member kind="function" virtualness="pure">
      <type>virtual Cell *</type>
      <name>getCell</name>
      <anchorfile>classHurricane_1_1Entity.html</anchorfile>
      <anchor>a42bdf015f583be477cc54b48652b1007</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Box</type>
      <name>getBoundingBox</name>
      <anchorfile>classHurricane_1_1Entity.html</anchorfile>
      <anchor>ad834f8ce33a08a13e2a88446696e63e7</anchor>
      <arglist>() const =0</arglist>
    </member>
    <docanchor file="classHurricane_1_1Entity.html" title="Introduction">secEntityIntro</docanchor>
    <docanchor file="classHurricane_1_1Entity.html" title="Unique Identifier">secEntityId</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Error</name>
    <filename>classHurricane_1_1Error.html</filename>
    <base>Hurricane::Exception</base>
    <member kind="function">
      <type></type>
      <name>Error</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>ab58387c890740ed8082532c5342f2d03</anchor>
      <arglist>(const string &amp;reason)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Error</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>a98f50dcce8258982d450e8f5f79cff38</anchor>
      <arglist>(const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Error</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>a33e4d2a1ea71be6395dc2716b68378c8</anchor>
      <arglist>(int code, const string &amp;reason)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Error</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>a32ccc14fe29d7d2a7b5fe66ee0a3845c</anchor>
      <arglist>(int code, const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Error</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>a7d90d5f5727dab2a9cc0a6427fb2b084</anchor>
      <arglist>(const Error &amp;error)</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>getReason</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>a1a18927a2d4eb2b0b0acfc2908be7008</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getCode</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>a1ba11c6ba6eff9fdf2923520fe80a6b2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>textWhere</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>ab3b8bb521802f332340eaf0b37eb1dfc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>htmlWhere</name>
      <anchorfile>classHurricane_1_1Error.html</anchorfile>
      <anchor>afafcbeae105f75906c7c45024de41c18</anchor>
      <arglist>() const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Error.html" title="Printing format">secErrorPrintingFormat</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Exception</name>
    <filename>classHurricane_1_1Exception.html</filename>
    <member kind="function">
      <type>string</type>
      <name>what</name>
      <anchorfile>classHurricane_1_1Exception.html</anchorfile>
      <anchor>a6d8036af345628567494eeab9c8e2e3a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>textWhat</name>
      <anchorfile>classHurricane_1_1Exception.html</anchorfile>
      <anchor>a19b5d24e8f02bb99625a5206638c44e8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>htmlWhat</name>
      <anchorfile>classHurricane_1_1Exception.html</anchorfile>
      <anchor>a2582e6c3acb236e24f48cb873f64e3e9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setTextTranslator</name>
      <anchorfile>classHurricane_1_1Exception.html</anchorfile>
      <anchor>a1a57fbbc4b57a014558ba31d18ec9b62</anchor>
      <arglist>(const TextTranslator &amp;translator)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setHtmlTranslator</name>
      <anchorfile>classHurricane_1_1Exception.html</anchorfile>
      <anchor>a0effe808df00f4efe10925131304b8d0</anchor>
      <arglist>(const TextTranslator &amp;translator)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Exception.html" title="Introduction">secExceptionIntro</docanchor>
    <docanchor file="classHurricane_1_1Exception.html" title="Example">secExceptionExample</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Filter</name>
    <filename>classHurricane_1_1Filter.html</filename>
    <templarg></templarg>
    <member kind="function">
      <type>GenericFilter&lt; Type &gt;</type>
      <name>operator!</name>
      <anchorfile>classHurricane_1_1Filter.html</anchorfile>
      <anchor>a90c1a8c4caf6c6018ff50f5b9754e061</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Filter&lt; Type &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Filter.html</anchorfile>
      <anchor>a596cad421801115efbc5c541f8d29e0b</anchor>
      <arglist>() const =0</arglist>
      <docanchor file="classHurricane_1_1Filter.html" title="Remarks">secFilterRemarks</docanchor>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>accept</name>
      <anchorfile>classHurricane_1_1Filter.html</anchorfile>
      <anchor>aeaa771f17950fe05273c471ccfffb7f7</anchor>
      <arglist>(Type type) const =0</arglist>
    </member>
    <docanchor file="classHurricane_1_1Filter.html" title="Introduction">secFilterIntro</docanchor>
    <docanchor file="classHurricane_1_1Filter.html" title="Simple Example">secFilterSimpleExample</docanchor>
    <docanchor file="classHurricane_1_1Filter.html" title="Complex Example">secFilterComplexExample</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::GenericCollection</name>
    <filename>classHurricane_1_1GenericCollection.html</filename>
    <templarg></templarg>
    <base>Hurricane::Collection</base>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a177ca321f1b7761c8b000a59051ba9f9</anchor>
      <arglist>(const Collection&lt; Type &gt; &amp;collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a9b77dc014864c2248f31b9dfee242d25</anchor>
      <arglist>(const GenericCollection&lt; Type &gt; &amp;genericCollection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a8e4f70213efb85c0ba802b7de9c03b32</anchor>
      <arglist>(Collection&lt; Type &gt; *collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a177ca321f1b7761c8b000a59051ba9f9</anchor>
      <arglist>(const Collection&lt; Type &gt; &amp;collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a9b77dc014864c2248f31b9dfee242d25</anchor>
      <arglist>(const GenericCollection&lt; Type &gt; &amp;genericCollection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a8e4f70213efb85c0ba802b7de9c03b32</anchor>
      <arglist>(Collection&lt; Type &gt; *collection)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GenericCollection&lt; Element &gt;</name>
    <filename>classHurricane_1_1GenericCollection.html</filename>
    <base>Collection&lt; Element &gt;</base>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a177ca321f1b7761c8b000a59051ba9f9</anchor>
      <arglist>(const Collection&lt; Element &gt; &amp;collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a9b77dc014864c2248f31b9dfee242d25</anchor>
      <arglist>(const GenericCollection&lt; Element &gt; &amp;genericCollection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a8e4f70213efb85c0ba802b7de9c03b32</anchor>
      <arglist>(Collection&lt; Element &gt; *collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a177ca321f1b7761c8b000a59051ba9f9</anchor>
      <arglist>(const Collection&lt; Element &gt; &amp;collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a9b77dc014864c2248f31b9dfee242d25</anchor>
      <arglist>(const GenericCollection&lt; Element &gt; &amp;genericCollection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericCollection</name>
      <anchorfile>classHurricane_1_1GenericCollection.html</anchorfile>
      <anchor>a8e4f70213efb85c0ba802b7de9c03b32</anchor>
      <arglist>(Collection&lt; Element &gt; *collection)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::GenericFilter</name>
    <filename>classHurricane_1_1GenericFilter.html</filename>
    <templarg></templarg>
    <base>Hurricane::Filter</base>
    <member kind="function">
      <type></type>
      <name>GenericFilter</name>
      <anchorfile>classHurricane_1_1GenericFilter.html</anchorfile>
      <anchor>a1aae208fe9937dd3a6f706ceb8b3b9b4</anchor>
      <arglist>(const Filter&lt; Type &gt; &amp;filter)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericFilter</name>
      <anchorfile>classHurricane_1_1GenericFilter.html</anchorfile>
      <anchor>adf44866e7507f45dd0d612743f2d9a71</anchor>
      <arglist>(const GenericFilter&lt; Type &gt; &amp;genericFilter)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericFilter</name>
      <anchorfile>classHurricane_1_1GenericFilter.html</anchorfile>
      <anchor>aac847f0c0d6ee640c54847e374287fe1</anchor>
      <arglist>(Filter&lt; Type &gt; *filter)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::GenericLocator</name>
    <filename>classHurricane_1_1GenericLocator.html</filename>
    <templarg></templarg>
    <base>Hurricane::Locator</base>
    <member kind="function">
      <type></type>
      <name>GenericLocator</name>
      <anchorfile>classHurricane_1_1GenericLocator.html</anchorfile>
      <anchor>a878eae335b3f60fc66ec6362d84c2b3e</anchor>
      <arglist>(const Locator&lt; Type &gt; &amp;locator)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericLocator</name>
      <anchorfile>classHurricane_1_1GenericLocator.html</anchorfile>
      <anchor>aa314dea86573d1cee1d1eea8ac2ab49e</anchor>
      <arglist>(const GenericLocator &amp;genericLocator)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericLocator</name>
      <anchorfile>classHurricane_1_1GenericLocator.html</anchorfile>
      <anchor>a4706b6502b806f90f2374df76791a729</anchor>
      <arglist>(Locator&lt; Type &gt; *locator)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>GenericLocator&lt; Element &gt;</name>
    <filename>classHurricane_1_1GenericLocator.html</filename>
    <base>Locator&lt; Element &gt;</base>
    <member kind="function">
      <type></type>
      <name>GenericLocator</name>
      <anchorfile>classHurricane_1_1GenericLocator.html</anchorfile>
      <anchor>a878eae335b3f60fc66ec6362d84c2b3e</anchor>
      <arglist>(const Locator&lt; Element &gt; &amp;locator)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericLocator</name>
      <anchorfile>classHurricane_1_1GenericLocator.html</anchorfile>
      <anchor>aa314dea86573d1cee1d1eea8ac2ab49e</anchor>
      <arglist>(const GenericLocator &amp;genericLocator)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>GenericLocator</name>
      <anchorfile>classHurricane_1_1GenericLocator.html</anchorfile>
      <anchor>a4706b6502b806f90f2374df76791a729</anchor>
      <arglist>(Locator&lt; Element &gt; *locator)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Go</name>
    <filename>classHurricane_1_1Go.html</filename>
    <base>Hurricane::Entity</base>
    <member kind="function">
      <type>bool</type>
      <name>isMaterialized</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>a0fd2574d3e2e0157230209acdc1b8aa6</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>materialize</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>a8251eba8fabfca57f574921c6c85728f</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>unmaterialize</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>af79318dc9cbbed85aea1bb8f16eb9724</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>invalidate</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>a5ee451e118fe8cace16989c0f3a6d855</anchor>
      <arglist>(bool propagateFlag=true)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>translate</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>a54c4351dbbf4045e1aa89f06bb893402</anchor>
      <arglist>(const DbU::Unit &amp;dx, const DbU::Unit &amp;dy)=0</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>autoMaterializationIsDisabled</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>a1057be4198a7b64c32a2ac3c7d560014</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>enableAutoMaterialization</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>ab0b1ca3c606247e1ebd7cab8fa828b04</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>disableAutoMaterialization</name>
      <anchorfile>classHurricane_1_1Go.html</anchorfile>
      <anchor>a0d49d22a3788e8001e58152e62b9f3cc</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1Go.html" title="Introduction">secGoIntro</docanchor>
    <docanchor file="classHurricane_1_1Go.html" title="Materialization">secGoMaterialization</docanchor>
    <docanchor file="classHurricane_1_1Go.html" title="Update sessions">secGoUpdateSessions</docanchor>
    <docanchor file="classHurricane_1_1Go.html" title="Construction and destruction">secGoConstructionAndDestruction</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Hook</name>
    <filename>classHurricane_1_1Hook.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual Component *</type>
      <name>getComponent</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>ab420305aa59b8ff10d59678363de2511</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getNextHook</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>a03044fa995d6d784d6c441927ca8af04</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getPreviousHook</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>ad69ebbbf3d64343aca23ca435f24c624</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getMasterHook</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>af18e0531df4ed14b64cf058b780aee46</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getNextMasterHook</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>a0923a5a2d0a7ee0458876eed72008e46</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getPreviousMasterHook</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>a80bf5cdd4e81952064f1be94fe10188f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hooks</type>
      <name>getHooks</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>a2def96fbcd444bebc16e589357c2a779</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hooks</type>
      <name>getSlaveHooks</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>ad3c977e4f253a18cf24dfe4a6fd24cb1</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isMaster</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>af0940eb0761f05df0b82c4198e22a01c</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isAttached</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>acd62c7de2c023a1013d5a728159d068d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>detach</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>a83f5beb5092e97947d24bd18adb33db1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>attach</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>aacc4dacd0d128b35fd15546bc6dde3c3</anchor>
      <arglist>(Hook *hook)</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Hook.html</anchorfile>
      <anchor>a7b98f0796a9080495472d574a23bcca0</anchor>
      <arglist>(Hook *hook)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Hook.html" title="Introduction">secHookIntro</docanchor>
    <docanchor file="classHurricane_1_1Hook.html" title="Rings">secHookRings</docanchor>
    <docanchor file="classHurricane_1_1Hook.html" title="Master and Slave hook types">secHookMasterAndSlaveHookTypes</docanchor>
    <docanchor file="classHurricane_1_1Hook.html" title="Explicit connections">secHookExplicitConnections</docanchor>
    <docanchor file="classHurricane_1_1Hook.html" title="Implicit connections">secHookImplicitConnections</docanchor>
    <docanchor file="classHurricane_1_1Hook.html" title="Concepts of HyperHooks and HyperRings">secHookConceptsOfHyperhooksAndHyperrings</docanchor>
    <docanchor file="classHurricane_1_1Hook.html" title="Constructor and Destructor">secHookConstructorAndDestructor</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Horizontal</name>
    <filename>classHurricane_1_1Horizontal.html</filename>
    <base>Hurricane::Segment</base>
    <member kind="typedef">
      <type>Segment</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Horizontal.html</anchorfile>
      <anchor>a43266e3530dc5872f4eabf16eba86bdb</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getDxSource</name>
      <anchorfile>classHurricane_1_1Horizontal.html</anchorfile>
      <anchor>a7fdafaa2a7e931413efb2c1fc021f4a8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getDxTarget</name>
      <anchorfile>classHurricane_1_1Horizontal.html</anchorfile>
      <anchor>aab807785755e4229f215aee3f3b16941</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setY</name>
      <anchorfile>classHurricane_1_1Horizontal.html</anchorfile>
      <anchor>a794aa68157beb2d04816a5f4e9160187</anchor>
      <arglist>(const DbU::Unit &amp;y)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>translate</name>
      <anchorfile>classHurricane_1_1Horizontal.html</anchorfile>
      <anchor>a3d01a89cb24cdb29dda86ac9910c7530</anchor>
      <arglist>(const DbU::Unit &amp;dy)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Horizontal *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Horizontal.html</anchorfile>
      <anchor>a15f13508993b6c0219fb944fe1141c3f</anchor>
      <arglist>(Net *net, const Layer *layer, const DbU::Unit &amp;y, const DbU::Unit &amp;width=0, const DbU::Unit &amp;dxSource=0, const DbU::Unit &amp;dxTarget=0)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Horizontal *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Horizontal.html</anchorfile>
      <anchor>ac30e0cbdfdd1f2cb02f000380652daf7</anchor>
      <arglist>(Component *source, Component *target, const Layer *layer, const DbU::Unit &amp;y, const DbU::Unit &amp;width=0, const DbU::Unit &amp;dxSource=0, const DbU::Unit &amp;dxTarget=0)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Horizontal.html" title="Introduction">secHorizontalIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::HyperNet</name>
    <filename>classHurricane_1_1HyperNet.html</filename>
    <member kind="function">
      <type></type>
      <name>HyperNet</name>
      <anchorfile>classHurricane_1_1HyperNet.html</anchorfile>
      <anchor>a30bdc04b4dece8bdef66361fe4469175</anchor>
      <arglist>(const Occurrence &amp;occurrence)</arglist>
    </member>
    <member kind="function">
      <type>const Occurrence &amp;</type>
      <name>getNetOccurrence</name>
      <anchorfile>classHurricane_1_1HyperNet.html</anchorfile>
      <anchor>a327eab6dda243836becde745bfc53efa</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getCell</name>
      <anchorfile>classHurricane_1_1HyperNet.html</anchorfile>
      <anchor>a5a818c5887d1d8dd2e0a59e8a57c02d7</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Occurrences</type>
      <name>getNetOccurrences</name>
      <anchorfile>classHurricane_1_1HyperNet.html</anchorfile>
      <anchor>a02180e650b1f2e5b87bf4774a5799ebc</anchor>
      <arglist>(bool doExtraction=false, bool allowInterruption=false) const</arglist>
    </member>
    <member kind="function">
      <type>Occurrences</type>
      <name>getNetOccurrencesUnder</name>
      <anchorfile>classHurricane_1_1HyperNet.html</anchorfile>
      <anchor>ab278267a5f1d91bd22bc7fe411b3cfb0</anchor>
      <arglist>(Box area, bool doExtraction=false, bool allowInterruption=false) const</arglist>
    </member>
    <docanchor file="classHurricane_1_1HyperNet.html" title="Introduction">secHyperNetIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Initializer</name>
    <filename>classHurricane_1_1Initializer.html</filename>
    <templarg></templarg>
    <member kind="function">
      <type></type>
      <name>Initializer</name>
      <anchorfile>classHurricane_1_1Initializer.html</anchorfile>
      <anchor>ab80fdb7c17aaf5bd3facdf3f0f9d12ae</anchor>
      <arglist>(unsigned int order)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Initializer.html" title="Initializer Mechanism">secInitializerMechanism</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Instance</name>
    <filename>classHurricane_1_1Instance.html</filename>
    <base>Hurricane::Go</base>
    <class kind="class">Hurricane::Instance::PlacementStatus</class>
    <member kind="typedef">
      <type>Go</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a227e1f98670e466328ca95fe45546590</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>aa48280b4d7127d283c89983cf7a42c23</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getMasterCell</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>ad08a772e5e36582070cdc407cfcc1a64</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Transformation &amp;</type>
      <name>getTransformation</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a5042051d648fd93548dc6c5e14782645</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Plug *</type>
      <name>getPlug</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a1afe07edc0374ddfb505b4973c902834</anchor>
      <arglist>(const Net *masterNet) const</arglist>
    </member>
    <member kind="function">
      <type>Plugs</type>
      <name>getPlugs</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a5433b64eed99f9a099004490fae6d8f4</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Plugs</type>
      <name>getConnectedPlugs</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a18beeab0def83c20e25a710b30dd8ca9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Plugs</type>
      <name>getUnconnectedPlugs</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a9622b8b961f459469c275b3dafe1733c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Path</type>
      <name>getPath</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a4d13f5b9294d0361b724b5824fd86378</anchor>
      <arglist>(const Path &amp;tailPath=Path()) const</arglist>
    </member>
    <member kind="function">
      <type>Box</type>
      <name>getAbutmentBox</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a29bedbd05939bf43757ef036bb506d01</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>ae3b67792d1659f1a20c6533b8843b905</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setTransformation</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a8890d2e1b2ba2542997454297e63512f</anchor>
      <arglist>(const Transformation &amp;transformation)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setMasterCell</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>a9f626fd058c21ffc2ed5bfee8d29a853</anchor>
      <arglist>(Cell *masterCell, bool secureFlag=true)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>uniquify</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>adf28fcd01f6ff89c5435e83482f66d4c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Instance *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>ac5e111eef5767762e00f21fcd7a35702</anchor>
      <arglist>(Cell *cloneCell) const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Instance *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>ae130b66536e4536ba8852fb79abfb89e</anchor>
      <arglist>(Cell *cell, const Name &amp;name, Cell *masterCell, bool secureFlag=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Instance *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>ad5784305151e45c9d949a74bd85aaa36</anchor>
      <arglist>(Cell *cell, const Name &amp;name, Cell *masterCell, const Transformation &amp;transformation, const PlacementStatus &amp;placementstatus, bool secureFlag=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static InstanceFilter</type>
      <name>getIsUnderFilter</name>
      <anchorfile>classHurricane_1_1Instance.html</anchorfile>
      <anchor>ae2bc936dfecfaf70a0052959b4b2861e</anchor>
      <arglist>(const Box &amp;area)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Instance.html" title="Introduction">secInstanceIntro</docanchor>
    <docanchor file="classHurricane_1_1Instance.html" title="Placement Status">secInstancePlacement</docanchor>
    <docanchor file="classHurricane_1_1Instance.html" title="Predefined filters">secInstancePredefinedFilters</docanchor>
    <docanchor file="classHurricane_1_1Instance.html" title="Instance Destruction">secInstanceDestroy</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Interruption</name>
    <filename>classHurricane_1_1Interruption.html</filename>
    <base>Hurricane::Exception</base>
    <member kind="typedef">
      <type>Exception</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Interruption.html</anchorfile>
      <anchor>a47ecad9b4b2bd34a21de4a0d6fdf1f5d</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Interruption</name>
      <anchorfile>classHurricane_1_1Interruption.html</anchorfile>
      <anchor>ae6375068418b2898b95c3cb7974e051c</anchor>
      <arglist>(const string &amp;reason, int code=0)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Interruption</name>
      <anchorfile>classHurricane_1_1Interruption.html</anchorfile>
      <anchor>ad84fb4212bce9f3a85b90b4a969226a6</anchor>
      <arglist>(const Interruption &amp;interruption)</arglist>
    </member>
    <member kind="function">
      <type>Interruption &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Interruption.html</anchorfile>
      <anchor>a3c528402a234e354e508b5b7512475bc</anchor>
      <arglist>(const Interruption &amp;interruption)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Interruption.html" title="Introduction">secInterruptionIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Interval</name>
    <filename>classHurricane_1_1Interval.html</filename>
    <member kind="function">
      <type></type>
      <name>Interval</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a02b04ad7ca380422098992fa8ff5f546</anchor>
      <arglist>(bool makeEmpty=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Interval</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>ace4173705b4dbcf6c00cd83bb61c4d43</anchor>
      <arglist>(const DbU::Unit &amp;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Interval</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a35e2ddc881a5b0c3ff8003d52f6298bb</anchor>
      <arglist>(const DbU::Unit &amp;v1, const DbU::Unit &amp;v2)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Interval</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a2db3923eb057dd19f5320d93a09750d9</anchor>
      <arglist>(const Interval &amp;)</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a337b424cea8024f574726c3a2e4935b8</anchor>
      <arglist>(const Interval &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a1b022ac0ad975f168ac2b2689e6368c3</anchor>
      <arglist>(const Interval &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a2e5a64c485269fb08fb762e1eb3bc374</anchor>
      <arglist>(const Interval &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getVMin</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a6e0deb1b38065375a78c7fd6885b5909</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getVMax</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a2f5ec659fde913492f89dc215001acb2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getCenter</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a6d12d0404054c7ccadab1afa6683a561</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getSize</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a61d877fee3986f93c357910cd63f1caa</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHalfSize</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>abe66d75c0854ca0a76189801f0f7d0e3</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Interval</type>
      <name>getUnion</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>ac50b0e28faf03b54f81af109d942b569</anchor>
      <arglist>(const Interval &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>Interval</type>
      <name>getIntersection</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a73130b484cf43ff3b48488780a926ead</anchor>
      <arglist>(const Interval &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isEmpty</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a5bf0292743d02f861a194c48c823c7ce</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isPonctual</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>acfc27bb7442f359db7d04c72fa8edeb8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>contains</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a84beba7ba34552e12e6cb9e462a94765</anchor>
      <arglist>(const DbU::Unit &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>contains</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>af613eb138f2035f50cba47057a074b2e</anchor>
      <arglist>(const Interval &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>intersect</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>af4862b82fe5b37cdb3986a3b05245469</anchor>
      <arglist>(const Interval &amp;, bool strict=false) const</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>makeEmpty</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a1e171021dcd5c0dc7e8afb0b2324c5ee</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>inflate</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a142c3ec37ebe74c253b3fe0039ef2143</anchor>
      <arglist>(const DbU::Unit &amp;dv)</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>inflate</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>ac311880a39d8e3db79bcbc5d3bb341a6</anchor>
      <arglist>(const DbU::Unit &amp;dvMin, const DbU::Unit &amp;dvMax)</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>ab37a2b3ad247a0a5a4e4946d2b777bec</anchor>
      <arglist>(const DbU::Unit &amp;)</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a99c17b60766c1146ad380ac9981008f7</anchor>
      <arglist>(const Interval &amp;)</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>intersection</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>a0eeaaa7eb5b4ade89719c57a2c284909</anchor>
      <arglist>(const DbU::Unit &amp;vMin, const DbU::Unit &amp;vMax)</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>intersection</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>aea3de219c9e8316e19d71d44428b8dc4</anchor>
      <arglist>(const Interval &amp;)</arglist>
    </member>
    <member kind="function">
      <type>Interval &amp;</type>
      <name>translate</name>
      <anchorfile>classHurricane_1_1Interval.html</anchorfile>
      <anchor>acf0aab51a74fe1216bfe112999066466</anchor>
      <arglist>(const DbU::Unit &amp;)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Interval.html" title="Introduction">secIntervalIntro</docanchor>
    <docanchor file="classHurricane_1_1Interval.html" title="Remark">secIntervalRemark</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::JsonObject</name>
    <filename>classHurricane_1_1JsonObject.html</filename>
    <member kind="function">
      <type></type>
      <name>JsonObject</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a57d9c022204b30d4b253c2588f2c1215</anchor>
      <arglist>(unsigned long flags)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>isDummy</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a8f0949b75f5900e4ef87196e949cfa6b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual std::string</type>
      <name>getTypeName</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a947e1c3f8dbae63bb2d086b5b827a2a5</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>getStackName</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a34c4e38611238021df8cc69fc32f5e48</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>check</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a6ac9230d36590f83fbe1561b6c1bb0e5</anchor>
      <arglist>(JsonStack &amp;, std::string fname) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>has</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>ac0b8816eab2cbcabf18d5421a59aa780</anchor>
      <arglist>(const std::string &amp;key) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>add</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a39ed04c118b19f1b6010b7f3336c360f</anchor>
      <arglist>(const std::string &amp;key, std::type_index tid)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a6435e5a8655a6b42b61d55e7fd673c65</anchor>
      <arglist>(const std::string &amp;key)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>get</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>af599fb8e2f3aca9c00dc628927367cb8</anchor>
      <arglist>(JsonStack &amp;, const std::string &amp;key) const</arglist>
    </member>
    <member kind="function">
      <type>T *</type>
      <name>jget</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a7a3fe903ceb00a3ebff66d1696edf682</anchor>
      <arglist>(JsonStack &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>copyAttrs</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a380ea9ac8689e1e9cad892edf0024c08</anchor>
      <arglist>(const JsonObject *, bool reset=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clear</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a5f11139263926dbd8fe87b9c4480bdae</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>aa4d6d0502f0f25e614ef50ac3dd76263</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>aeda98a478720cf29e532e2a11f6f54c5</anchor>
      <arglist>(const std::string &amp;)</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>getObject</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>ab1a1aaaa583d5c8f7adbc9c9d0eeeebf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setObject</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>aa8e23d2445b90cced19a97d50372314c</anchor>
      <arglist>(T)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isBound</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a4bb80844dcd175ad16cf92ee29064850</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual JsonObject *</type>
      <name>clone</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a939cfbae43f7b0d994648d07bdba2b32</anchor>
      <arglist>(unsigned long flags) const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>toData</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a57a845ca64ac8912b35c4dbf75723af6</anchor>
      <arglist>(JsonStack &amp;)</arglist>
    </member>
    <member kind="function">
      <type>unsigned int</type>
      <name>presetId</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a3819d0c96ee99277e4cc8d349dc4155c</anchor>
      <arglist>(JsonStack &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>update</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>ace589494452d9d797506286613642cd4</anchor>
      <arglist>(JsonStack &amp;, T)</arglist>
    </member>
    <member kind="function">
      <type>JsonObject *</type>
      <name>setFlags</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a5216e5f5d74f8dbf8665eef8a78b2fe1</anchor>
      <arglist>(unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>JsonObject *</type>
      <name>resetFlags</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a8346b1c958574275fe17f9c4cb40ee8b</anchor>
      <arglist>(unsigned long mask)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>issetFlags</name>
      <anchorfile>classHurricane_1_1JsonObject.html</anchorfile>
      <anchor>a267c55a1989a9907e25632d820681779</anchor>
      <arglist>(unsigned long mask) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::JsonStack</name>
    <filename>classHurricane_1_1JsonStack.html</filename>
    <member kind="function">
      <type>size_t</type>
      <name>size</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a80b1e4cab1a3c4cac83491d18c76534d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_back</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a189a4e74a62f898f5fe903a185181bc5</anchor>
      <arglist>(const std::string &amp;, T)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pop_back</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a668d3c1974efefb6ced90544b4703fe5</anchor>
      <arglist>(size_t count=1)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rhas</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a6416903ee046d6b8122fe2ffe8c0751b</anchor>
      <arglist>(const std::string &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>as</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>aed841c5329cfb5a72e7df326e995ce73</anchor>
      <arglist>(const std::string &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>as</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>acc35d32ee522ff75b64122c40ee71113</anchor>
      <arglist>(int) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>push_back_dbo</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>ab949c42fe63dcaad6a8e23954167a9a7</anchor>
      <arglist>(DBo *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>pop_back_dbo</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>adeeb2a0300fa248f1f3bff38c4095630</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>DBo *</type>
      <name>back_dbo</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a95f403acdc0124c279866bd92ef8500a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>vector&lt; JsonObject * &gt; &amp;</type>
      <name>jobjects</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a974ee9daf5dbf861ed50cae0a51a5cbb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>T</type>
      <name>getEntity</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>aadbd70d7d19d2eb0a6c9c86ca1583c76</anchor>
      <arglist>(unsigned int) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addEntity</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a421fdf4a3bab01d75f0aab292cc574d0</anchor>
      <arglist>(unsigned int jsonId, Entity *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>print</name>
      <anchorfile>classHurricane_1_1JsonStack.html</anchorfile>
      <anchor>a7e0364c8868b8396db10399a855b8aa1</anchor>
      <arglist>(std::ostream &amp;) const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Layer</name>
    <filename>classHurricane_1_1Layer.html</filename>
    <base>Hurricane::DBo</base>
    <member kind="typedef">
      <type>Hurricane::Mask&lt; unsigned long long &gt;</type>
      <name>Mask</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>af5277c670637bd5d910237e7afe01a91</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Technology *</type>
      <name>getTechnology</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>ae506b17bd7a245de622f8a8e9947629b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a3dc54f6efc60fddb8529599caa6b0f1f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Mask &amp;</type>
      <name>getMask</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a29b22c3b59cc24bf82449ad6c068ff1f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Mask &amp;</type>
      <name>getExtractMask</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>af009bbd89a8e8260122b7244bfa10349</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getMinimalSize</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>afed9a488bf20daaeed18874f2b16268e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getMinimalSpacing</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a6a03f9f40ca855d33763497162414062</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual BasicLayers</type>
      <name>getBasicLayers</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a7e953c126a02952e3a0b0d32f37e2ae0</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const Layer *</type>
      <name>getTop</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a5f7c43a29f3dd02a9ebccbcbf91d6727</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const Layer *</type>
      <name>getBottom</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a4dab4552a219d2d900ed0b1245dc6580</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const Layer *</type>
      <name>getOpposite</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a69e76c09a56260169c4f5c145a35a47f</anchor>
      <arglist>(const Layer *) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getMetalAbove</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>ac32eff2dc51180660fe9b4ce17cae42c</anchor>
      <arglist>(bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getMetalBelow</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a4bc8358f67c1a1a9b2581e4b3732108c</anchor>
      <arglist>(bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getCutAbove</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>ad9fca94fed1837e3e80e9b6445822b7d</anchor>
      <arglist>(bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getCutBelow</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a983956c8127688390f978cc5bd0d768d</anchor>
      <arglist>(bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>above</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>abbd13bf66cf75dd6445d0353987119f3</anchor>
      <arglist>(const Layer *layer) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>below</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a090f8697946f721351a626052af25027</anchor>
      <arglist>(const Layer *layer) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>contains</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>af63dd0a48e2a3514a1cdaccd4586bad8</anchor>
      <arglist>(const Layer *layer) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>intersect</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>adbea0bafaa87b033efdaa98bf2709182</anchor>
      <arglist>(const Layer *layer) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>ab93809f19bc360f58d35e91438ef2f87</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setMinimalSize</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a400d17fe999c0080bb50489948986fe7</anchor>
      <arglist>(const DbU::Unit &amp;minimalSize)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setMinimalSpacing</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a81a8a24526e8535fba5a35cdcc194a8f</anchor>
      <arglist>(const DbU::Unit &amp;minimalSpacing)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setEnclosure</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a04e9c983525d074508d7e10107c1c3c7</anchor>
      <arglist>(const BasicLayer *layer, DbU::Unit, uint32_t flags)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setExtentionCap</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a55c7b39e000442ea36a0774d26b7fbde</anchor>
      <arglist>(const BasicLayer *layer, DbU::Unit)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setExtentionWidth</name>
      <anchorfile>classHurricane_1_1Layer.html</anchorfile>
      <anchor>a7a6943dbcb3403aff34056cd5de00e66</anchor>
      <arglist>(const BasicLayer *layer, DbU::Unit)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Layer.html" title="Introduction">secLayerIntro</docanchor>
    <docanchor file="classHurricane_1_1Layer.html" title="BasicLayer Creation Ordering">secBasicLayerOrder</docanchor>
    <docanchor file="classHurricane_1_1Layer.html" title="Looking Up a Layer from a Mask">secLayerLookup</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Library</name>
    <filename>classHurricane_1_1Library.html</filename>
    <base>Hurricane::DBo</base>
    <member kind="typedef">
      <type>DBo</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a30ef446b2da0d405bdf4e11ce67b160f</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>DataBase *</type>
      <name>getDataBase</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a657cbf1ac426ef6def0b5b51f80ed248</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Library *</type>
      <name>getLibrary</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a5299d19afc96c535d557b86ba42eaa82</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a13a9c4d0c43e2e1df09b7c0ee59f577f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Library *</type>
      <name>getLibrary</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a8589e1ff3db5ef288c3027ceef28636b</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>Libraries</type>
      <name>getLibraries</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a43b3703b939b7e70261d1f9319db2bb0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getCell</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a2e6bb294c611db79e00a5a6ea00974d5</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>Cells</type>
      <name>getCells</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>aae3e47aef08e50f9858fb79537f6eb41</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a1181e4e87f42749bdfda253cad658ea9</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Library *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>af304234d0347128300df5ad09801229d</anchor>
      <arglist>(DataBase *dataBase, const Name &amp;name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Library *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Library.html</anchorfile>
      <anchor>a36bc1af0e48307180be81a81d462650e</anchor>
      <arglist>(Library *library, const Name &amp;name)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Library.html" title="Introduction">secLibraryIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::ListCollection</name>
    <filename>classHurricane_1_1ListCollection.html</filename>
    <templarg></templarg>
    <base>Collection&lt; Element &gt;</base>
    <member kind="function">
      <type></type>
      <name>ListCollection</name>
      <anchorfile>classHurricane_1_1ListCollection.html</anchorfile>
      <anchor>a11d0b27c9b01f16fe9ac9da091575e7c</anchor>
      <arglist>(const ElementList *elementList=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>ListCollection</name>
      <anchorfile>classHurricane_1_1ListCollection.html</anchorfile>
      <anchor>a11d0b27c9b01f16fe9ac9da091575e7c</anchor>
      <arglist>(const ElementList *elementList=NULL)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Locator</name>
    <filename>classHurricane_1_1Locator.html</filename>
    <templarg></templarg>
    <member kind="function" virtualness="pure">
      <type>virtual Type</type>
      <name>getElement</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>aa2202b4cf461a7c3b666da10bc96219f</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Type &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>a83779aa300e3de011bf3b93be8a48d83</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>abb6e5255372e22e31bf0a8e4cae93f87</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>progress</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>ad8d72c1625a343a50520792c96fa1ca1</anchor>
      <arglist>()=0</arglist>
    </member>
    <docanchor file="classHurricane_1_1Locator.html" title="Introduction">secLocatorIntro</docanchor>
    <docanchor file="classHurricane_1_1Locator.html" title="General concepts">secLocatorGeneralConcepts</docanchor>
    <docanchor file="classHurricane_1_1Locator.html" title="Usage examples">secLocatorUsageExamples</docanchor>
  </compound>
  <compound kind="class">
    <name>Locator&lt; Cell * &gt;</name>
    <filename>classHurricane_1_1Locator.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual Cell *</type>
      <name>getElement</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>aa2202b4cf461a7c3b666da10bc96219f</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Cell * &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>a83779aa300e3de011bf3b93be8a48d83</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>abb6e5255372e22e31bf0a8e4cae93f87</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>progress</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>ad8d72c1625a343a50520792c96fa1ca1</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Locator&lt; Element &gt;</name>
    <filename>classHurricane_1_1Locator.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual Element</type>
      <name>getElement</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>aa2202b4cf461a7c3b666da10bc96219f</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Element &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>a83779aa300e3de011bf3b93be8a48d83</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>abb6e5255372e22e31bf0a8e4cae93f87</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>progress</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>ad8d72c1625a343a50520792c96fa1ca1</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Locator&lt; ElType &gt;</name>
    <filename>classHurricane_1_1Locator.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual ElType</type>
      <name>getElement</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>aa2202b4cf461a7c3b666da10bc96219f</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; ElType &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>a83779aa300e3de011bf3b93be8a48d83</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>abb6e5255372e22e31bf0a8e4cae93f87</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>progress</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>ad8d72c1625a343a50520792c96fa1ca1</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Locator&lt; Hurricane::DBo * &gt;</name>
    <filename>classHurricane_1_1Locator.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual Hurricane::DBo *</type>
      <name>getElement</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>aa2202b4cf461a7c3b666da10bc96219f</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Hurricane::DBo * &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>a83779aa300e3de011bf3b93be8a48d83</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>abb6e5255372e22e31bf0a8e4cae93f87</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>progress</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>ad8d72c1625a343a50520792c96fa1ca1</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Locator&lt; Hurricane::Instance * &gt;</name>
    <filename>classHurricane_1_1Locator.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual Hurricane::Instance *</type>
      <name>getElement</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>aa2202b4cf461a7c3b666da10bc96219f</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; Hurricane::Instance * &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>a83779aa300e3de011bf3b93be8a48d83</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>abb6e5255372e22e31bf0a8e4cae93f87</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>progress</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>ad8d72c1625a343a50520792c96fa1ca1</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Locator&lt; SubType &gt;</name>
    <filename>classHurricane_1_1Locator.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual SubType</type>
      <name>getElement</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>aa2202b4cf461a7c3b666da10bc96219f</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual Locator&lt; SubType &gt; *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>a83779aa300e3de011bf3b93be8a48d83</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>abb6e5255372e22e31bf0a8e4cae93f87</anchor>
      <arglist>() const=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>progress</name>
      <anchorfile>classHurricane_1_1Locator.html</anchorfile>
      <anchor>ad8d72c1625a343a50520792c96fa1ca1</anchor>
      <arglist>()=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::MapCollection</name>
    <filename>classHurricane_1_1MapCollection.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <templarg></templarg>
    <base>Collection&lt; Element &gt;</base>
    <member kind="function">
      <type></type>
      <name>MapCollection</name>
      <anchorfile>classHurricane_1_1MapCollection.html</anchorfile>
      <anchor>a0b905fb46ced35815132e5eab62a8de1</anchor>
      <arglist>(const ElementMap *elementMap=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>MapCollection</name>
      <anchorfile>classHurricane_1_1MapCollection.html</anchorfile>
      <anchor>a0b905fb46ced35815132e5eab62a8de1</anchor>
      <arglist>(const ElementMap *elementMap=NULL)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::BasicLayer::Material</name>
    <filename>classHurricane_1_1BasicLayer_1_1Material.html</filename>
    <member kind="enumvalue">
      <name>nWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8acdf0cf84e1b081113add657e0a8bd49c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8abab32187e9c4013a84e39cb6283bcc92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac4e1ac9fce1c8328c415aebc024d1fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a73d7adbdda868d5e680a24ce3c20279e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>active</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ab67cfb3c192135ea8d52452a8932f7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>poly</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a506eb536d54e1005b664cc0f2c101670</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>cut</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac853c14f5754c0d802b5be0f8068b4cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>metal</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a9f5ac52339b7bd9bbf7cdac468c51924</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>blockage</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a0dd9279952186054fda92b8a97b253fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>other</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a43faff93bcb2788ccb23905cc0d07bec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>Code</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8acdf0cf84e1b081113add657e0a8bd49c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8abab32187e9c4013a84e39cb6283bcc92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac4e1ac9fce1c8328c415aebc024d1fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a73d7adbdda868d5e680a24ce3c20279e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>active</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ab67cfb3c192135ea8d52452a8932f7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>poly</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a506eb536d54e1005b664cc0f2c101670</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>cut</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac853c14f5754c0d802b5be0f8068b4cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>metal</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a9f5ac52339b7bd9bbf7cdac468c51924</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>blockage</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a0dd9279952186054fda92b8a97b253fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>other</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a43faff93bcb2788ccb23905cc0d07bec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8acdf0cf84e1b081113add657e0a8bd49c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8abab32187e9c4013a84e39cb6283bcc92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac4e1ac9fce1c8328c415aebc024d1fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a73d7adbdda868d5e680a24ce3c20279e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>active</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ab67cfb3c192135ea8d52452a8932f7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>poly</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a506eb536d54e1005b664cc0f2c101670</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>cut</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac853c14f5754c0d802b5be0f8068b4cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>metal</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a9f5ac52339b7bd9bbf7cdac468c51924</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>blockage</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a0dd9279952186054fda92b8a97b253fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>other</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a43faff93bcb2788ccb23905cc0d07bec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8acdf0cf84e1b081113add657e0a8bd49c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8abab32187e9c4013a84e39cb6283bcc92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac4e1ac9fce1c8328c415aebc024d1fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a73d7adbdda868d5e680a24ce3c20279e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>active</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ab67cfb3c192135ea8d52452a8932f7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>poly</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a506eb536d54e1005b664cc0f2c101670</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>cut</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac853c14f5754c0d802b5be0f8068b4cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>metal</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a9f5ac52339b7bd9bbf7cdac468c51924</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>blockage</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a0dd9279952186054fda92b8a97b253fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>other</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a43faff93bcb2788ccb23905cc0d07bec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>Code</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8acdf0cf84e1b081113add657e0a8bd49c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8abab32187e9c4013a84e39cb6283bcc92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac4e1ac9fce1c8328c415aebc024d1fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a73d7adbdda868d5e680a24ce3c20279e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>active</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ab67cfb3c192135ea8d52452a8932f7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>poly</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a506eb536d54e1005b664cc0f2c101670</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>cut</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac853c14f5754c0d802b5be0f8068b4cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>metal</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a9f5ac52339b7bd9bbf7cdac468c51924</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>blockage</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a0dd9279952186054fda92b8a97b253fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>other</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a43faff93bcb2788ccb23905cc0d07bec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8acdf0cf84e1b081113add657e0a8bd49c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pWell</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8abab32187e9c4013a84e39cb6283bcc92</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>nImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac4e1ac9fce1c8328c415aebc024d1fda</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>pImplant</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a73d7adbdda868d5e680a24ce3c20279e</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>active</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ab67cfb3c192135ea8d52452a8932f7b7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>poly</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a506eb536d54e1005b664cc0f2c101670</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>cut</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8ac853c14f5754c0d802b5be0f8068b4cf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>metal</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a9f5ac52339b7bd9bbf7cdac468c51924</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>blockage</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a0dd9279952186054fda92b8a97b253fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>other</name>
      <anchorfile>classHurricane_1_1BasicLayer_1_1Material.html</anchorfile>
      <anchor>a3e815440ad4b86b3569fa54ca06fc3e8a43faff93bcb2788ccb23905cc0d07bec</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Name</name>
    <filename>classHurricane_1_1Name.html</filename>
    <member kind="function">
      <type></type>
      <name>Name</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a42636ecb0d4d7d03eb881420a244038b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Name</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a754d54199d54c5e4568421c89f0682cb</anchor>
      <arglist>(const char *c)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Name</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a446df795ebe2e641710696bf775eb491</anchor>
      <arglist>(const string &amp;s)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Name</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a56ffee3e75dc343c7ec8b61102c1d3a2</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Name</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a1ce605ce16980334f93d7cc278984842</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Name &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>adcd165de286782c011acc31727adb4a1</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a3b728f0b8aa027639ebd47c60addf738</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>aff97f0bcf698ad76f6f3c9a4c4833cc3</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a9ce91a54cd340fb1e14baf56797f1577</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;=</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a9704f9fe4c605a86de13b6a8d90feab2</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a33bd981f4f6923a50c603cd06283032d</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&gt;=</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a241d0568f16c8ba60d4c5148be6a48b3</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>char</type>
      <name>operator[]</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a2e6f3869321016de8f98f2b35f136ab4</anchor>
      <arglist>(unsigned index) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isEmpty</name>
      <anchorfile>classHurricane_1_1Name.html</anchorfile>
      <anchor>a6c05cf200a0aeb95f98603fa0c9c9d4b</anchor>
      <arglist>() const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Name.html" title="Introduction">secNameIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Net</name>
    <filename>classHurricane_1_1Net.html</filename>
    <base>Hurricane::Entity</base>
    <class kind="class">Hurricane::Net::Direction</class>
    <class kind="class">Hurricane::Net::Type</class>
    <member kind="typedef">
      <type>Entity</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a3f1ac0fcb03638b2ffa9af6a9a58de15</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>unsigned</type>
      <name>Arity</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a3a242d929e0c733f90f3f69be8cc427b</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>aeba73ca641db371dde29baf348b58bba</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Arity &amp;</type>
      <name>getArity</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a78de2202fcf4f16024b4460ebb7dc907</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Type &amp;</type>
      <name>getType</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a0fa61dc0ccb67f384f03b35f83d391e7</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Direction &amp;</type>
      <name>getDirection</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>aa84245d734dfaa572660a1a2c1bfc56e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Point &amp;</type>
      <name>getPosition</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>abcfdec9c953d228845fdb9d75e8173cc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getX</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a1f8f4c4632614b84a1227a1da8310428</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getY</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>aa97899b408aa47ec22792b5c6d6e9216</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getComponents</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a1e2d7ef9bab15694870a605e514f26e8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Rubbers</type>
      <name>getRubbers</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a6ddbe2697a7fd7a7cd359f97b2ad0223</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>RoutingPads</type>
      <name>getRoutingPads</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a1078d55acf3efa0b3c23cd345cae87fa</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Plugs</type>
      <name>getPlugs</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a88322672c105405a61a78022359178aa</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Contacts</type>
      <name>getContacts</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a9c397596fe9ecbf674712c72e0b9010c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Segments</type>
      <name>getSegments</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>aadc5a9ef26d7a72f49fdf22452f3cc58</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Verticals</type>
      <name>getVerticals</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a97f32cf738af9cf107833ca81fe95db8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Horizontals</type>
      <name>getHorizontals</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>ad8553af888909e1c127e12e68bd000fb</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Pads</type>
      <name>getPads</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a564df07be5589bc72dd6eb944855aa2b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Plugs</type>
      <name>getSlavePlugs</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a9c835d2f071155521700921d816ac1fa</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Plugs</type>
      <name>getConnectedSlavePlugs</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a08560ffa6b0f5ecc442bf232486dd8ff</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Plugs</type>
      <name>getUnconnectedSlavePlugs</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>aad3f3ea88bdea914cab3f38bdcdb843d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isGlobal</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a332e9b311809f75fc0fa3a5f36acebcc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isExternal</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>aeeb3735dd7451bc0054dd68ac21aae47</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isLogical</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a9caf25bfa84478157d206979dd521ed4</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isClock</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a8807cc000bbbe1c340c71fcbfbb8fe33</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isSupply</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>ab8947fc6c5093341958b94148407c2a2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a1a39702b9f4d26ba29ad0dafcdddf840</anchor>
      <arglist>(Name name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setArity</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>af5dfdca4401902ee7e1e46a1a486da38</anchor>
      <arglist>(const Arity &amp;arity)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setGlobal</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a35c84afd9dade0cb715602bcf8ec8865</anchor>
      <arglist>(bool isGlobal)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setExternal</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a6a30bc8282ce7e4b936e73a11549fedf</anchor>
      <arglist>(bool isExternal)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setType</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a83f5ce12291b0ec5ab584d515dd8963c</anchor>
      <arglist>(const Type &amp;type)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDirection</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>ac33d13bb0ddc60f369d5bfcffc4bb0f8</anchor>
      <arglist>(const Direction &amp;direction)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setPosition</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a0a3a3232a74ebced14b14029c5199561</anchor>
      <arglist>(const Point &amp;position)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>materialize</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>ae46f7e4a9b00b265c06cb6e0ee00b806</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unmaterialize</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a9e53a3d54b61f7081263e6d7b4fa81b9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>merge</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a442f62d23364805f39816cd543284886</anchor>
      <arglist>(Net *net)</arglist>
    </member>
    <member kind="function">
      <type>Net *</type>
      <name>getClone</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a4bd27e6ae22606463491c28437c4068f</anchor>
      <arglist>(Cell *cloneCell)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Net *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>ac1524cea15e8eacbf992c0cfb9e481db</anchor>
      <arglist>(Cell *cell, const Name &amp;name)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsGlobalFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a1730ed1247cd9bce7fcf519ea60dc738</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsExternalFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a3af91a80e219e37e70229e61dfd385da</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsInternalFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a7a2d1c4ab84bf81a16e24557d2342ea5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsClockFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>afdb2269f3a88923c25264f6f785372a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsSupplyFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>ac241f44abf1f332004dd6103ee1dfa48</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsGlobalFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a1730ed1247cd9bce7fcf519ea60dc738</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsExternalFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a3af91a80e219e37e70229e61dfd385da</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsInternalFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>a7a2d1c4ab84bf81a16e24557d2342ea5</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsClockFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>afdb2269f3a88923c25264f6f785372a1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static NetFilter</type>
      <name>getIsSupplyFilter</name>
      <anchorfile>classHurricane_1_1Net.html</anchorfile>
      <anchor>ac241f44abf1f332004dd6103ee1dfa48</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1Net.html" title="Predefined filters">secNetPredefinedFilters</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::NotFilter</name>
    <filename>classHurricane_1_1NotFilter.html</filename>
    <templarg></templarg>
    <base>Hurricane::Filter</base>
    <member kind="function">
      <type></type>
      <name>NotFilter</name>
      <anchorfile>classHurricane_1_1NotFilter.html</anchorfile>
      <anchor>a8c75f2e192929c1b559f4ca876e47126</anchor>
      <arglist>(const Filter&lt; Type &gt; &amp;filter)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>NotFilter</name>
      <anchorfile>classHurricane_1_1NotFilter.html</anchorfile>
      <anchor>a232102dc584111a704e66b2ac793af86</anchor>
      <arglist>(const NotFilter&lt; Type &gt; &amp;notFilter)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Occurrence</name>
    <filename>classHurricane_1_1Occurrence.html</filename>
    <member kind="function">
      <type></type>
      <name>Occurrence</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>aa4162a36dc984d71caf962e55b991ed0</anchor>
      <arglist>(const Entity *entity=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Occurrence</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>afedb5d75781a9a4f0a19d37f0e8c88a8</anchor>
      <arglist>(const Entity *entity, const Path &amp;path)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Occurrence</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>affec5f25b9c2efa2bcde02e9c4833626</anchor>
      <arglist>(const Occurrence &amp;occurrence)</arglist>
    </member>
    <member kind="function">
      <type>Occurrence &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a389472d0d5b121a4e091511c8003cb47</anchor>
      <arglist>(const Occurrence &amp;occurrence)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>accae7f6611cb5985478d58fc793dc3e0</anchor>
      <arglist>(const Occurrence &amp;occurrence) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a39edad19edeef964e5340381360c0add</anchor>
      <arglist>(const Occurrence &amp;occurrence) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a2f1668ccce22799c99eb3a02e522c204</anchor>
      <arglist>(const Occurrence &amp;occurrence) const</arglist>
    </member>
    <member kind="function">
      <type>Entity *</type>
      <name>getEntity</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>ac121005d5d3bc0837f66c0de4265b0c4</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Path</type>
      <name>getPath</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>adeab556806a83e8cf3ad6ecc08f3a83e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getOwnerCell</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>affced95f35617150b5811c3784b20d93</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getMasterCell</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a5caeab69e23907909b3deb60ff26df15</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Property *</type>
      <name>getProperty</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>ab2b36b219037a2310f6527a35a9a266f</anchor>
      <arglist>(const Name &amp;name) const</arglist>
    </member>
    <member kind="function">
      <type>Properties</type>
      <name>getProperties</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>acbf59d6c01804e01f66d076c149abb49</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Box</type>
      <name>getBoundingBox</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a6c808dba6637c716075e0887c5f25518</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isValid</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>ade38e5da7eb5d8701cd3a8f252cdf62f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasProperty</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a0c1c6cfdf47f33166d108e2311d74e48</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>put</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>aaea0bdc4f5bb4012eb52f3abe20525be</anchor>
      <arglist>(Property *property)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a774404aa5eb01371f64cf5fda3f3ffbf</anchor>
      <arglist>(Property *property)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>removeProperty</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>a8d86755bf50cbc7fd2849b039a372b0a</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>clearProperties</name>
      <anchorfile>classHurricane_1_1Occurrence.html</anchorfile>
      <anchor>ae9b269d39f3f68645d6d396d7ab5d8b7</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1Occurrence.html" title="Introduction">secOccurrenceIntro</docanchor>
    <docanchor file="classHurricane_1_1Occurrence.html" title="Terminology">secOccurrenceTerminology</docanchor>
    <docanchor file="classHurricane_1_1Occurrence.html" title="Remarks">secOccurrenceRemarks</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Transformation::Orientation</name>
    <filename>classHurricane_1_1Transformation_1_1Orientation.html</filename>
  </compound>
  <compound kind="class">
    <name>Hurricane::Pad</name>
    <filename>classHurricane_1_1Pad.html</filename>
    <base>Hurricane::Component</base>
    <member kind="typedef">
      <type>Component</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Pad.html</anchorfile>
      <anchor>aa44130a291ce3cb878d749fbf3e5437e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Pad *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Pad.html</anchorfile>
      <anchor>a0fdf586f9f815d375f54b40bfa027b24</anchor>
      <arglist>(Net *net, const Layer *layer, const Box &amp;boundingBox)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Pad.html" title="Introduction">secPadIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Path</name>
    <filename>classHurricane_1_1Path.html</filename>
    <member kind="function">
      <type></type>
      <name>Path</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>ad3fe735dcb2ce630f89b98c039663c23</anchor>
      <arglist>(SharedPath *sharedPath=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Path</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>aa1a70f922b9b6a78fd3ac9b7bd94d158</anchor>
      <arglist>(Instance *instance)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Path</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a3197a114ed98117dde0f41d999917775</anchor>
      <arglist>(Instance *headInstance, const Path &amp;tailPath)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Path</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>add5812ab3bb9a4cf6dbe49d1e4e932cb</anchor>
      <arglist>(const Path &amp;headPath, Instance *tailInstance)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Path</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a6e3d331f5c5a0dcb91d10516a4beb6bc</anchor>
      <arglist>(Cell *cell, const string &amp;pathName)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Path</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a8db875f788013ec5ad8ed517cf1e1715</anchor>
      <arglist>(const Path &amp;path)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Path</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a6226639f50213598ffad86031afe69ff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Path &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a1355dd2d191d492a1b5e5180324a9f8f</anchor>
      <arglist>(const Path &amp;path)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a16a5b6529dd4424c55518ac9f687862f</anchor>
      <arglist>(const Path &amp;path) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a182e82a2bc3f41262e1e76fcdc5a0c1e</anchor>
      <arglist>(const Path &amp;path) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator&lt;</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a5bf33d2d9e3e7d46db770e26c09be90b</anchor>
      <arglist>(const Path &amp;path) const</arglist>
    </member>
    <member kind="function">
      <type>Instance *</type>
      <name>getHeadInstance</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>afddde635f302cee0a215ca364e9689b5</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Path</type>
      <name>getTailPath</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>af0b27566643cc252d9a0feb1709d3180</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Path</type>
      <name>getHeadPath</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>ae89034b297b27545cf3865e0cfa31f3d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Instance *</type>
      <name>getTailInstance</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a1f9350514c4751b54b2f01082a93e3bf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a97ff25c53f4e7bdacb7cb8a58adf6499</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getOwnerCell</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a0954eb842af9d863ea701aa0b681412e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getMasterCell</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a3f4a865f570375ec5b6e5cb487369696</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Instances</type>
      <name>getInstances</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>af820422a686ab35d611a6c1969e37e90</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Transformation</type>
      <name>getTransformation</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>af48dc47810f65e7aba6ee8f24ed8a09e</anchor>
      <arglist>(const Transformation &amp;transformation=Transformation()) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isEmpty</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>aeddd764b4b10c72630ee81119614935a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static char</type>
      <name>getNameSeparator</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>ac63015239df43b8c44a6d74a262eb3a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setNameSeparator</name>
      <anchorfile>classHurricane_1_1Path.html</anchorfile>
      <anchor>a505231a4bf7e8041c7a01e482505cd7a</anchor>
      <arglist>(char nameSeparator)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Path.html" title="Introduction">secPathIntro</docanchor>
    <docanchor file="classHurricane_1_1Path.html" title="Remarks">secPathRemarks</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::PhysicalRule</name>
    <filename>classHurricane_1_1PhysicalRule.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>isDouble</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>ad419b79177064cab06f7c8dbeccc4dce</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isDbU</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>af1e9eeb3499bfd66cedf5756e2d7e2a3</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isSymmetric</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>a56f3eed5835ad1c268638f0f813fc029</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>hasSteps</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>a8cc9f7f2dfc580e4f91b7f93ce8a26cc</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setSymmetric</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>afc1e61c5b2baa1126a2e253e02f40f40</anchor>
      <arglist>(bool)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>getDoubleValue</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>a89a0cd27aec78836500a5066c276bbf0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getValue</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>aa3548c58b42cd29cbe07d1a0289416a0</anchor>
      <arglist>(Hurricane::DbU::Unit length=0, bool hDir=true) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addValue</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>ada08351fb24f36a63f4e3a3c524000a2</anchor>
      <arglist>(double)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addValue</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>ac25990fc4aff5e5739ae9632f43d9bf8</anchor>
      <arglist>(Hurricane::DbU::Unit value, Hurricane::DbU::Unit maxLength)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addValue</name>
      <anchorfile>classHurricane_1_1PhysicalRule.html</anchorfile>
      <anchor>a51356a2e3e6cae11c8063b6a092f304d</anchor>
      <arglist>(Hurricane::DbU::Unit hValue, Hurricane::DbU::Unit vValue, Hurricane::DbU::Unit maxLength)</arglist>
    </member>
    <docanchor file="classHurricane_1_1PhysicalRule.html" title="Introduction">sPhysicalRuleIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Pin</name>
    <filename>classHurricane_1_1Pin.html</filename>
    <base>Hurricane::Contact</base>
  </compound>
  <compound kind="class">
    <name>Hurricane::Instance::PlacementStatus</name>
    <filename>classHurricane_1_1Instance_1_1PlacementStatus.html</filename>
    <member kind="enumvalue">
      <name>UNPLACED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfa3e19a0a1b3e8c8fd860164df7f935216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PLACED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfaf3589c11ecd7d5de63db24826b74d457</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FIXED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfa47be8a40f04081635fe24485ae7c6bd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>Code</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNPLACED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfa3e19a0a1b3e8c8fd860164df7f935216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PLACED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfaf3589c11ecd7d5de63db24826b74d457</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FIXED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfa47be8a40f04081635fe24485ae7c6bd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNPLACED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfa3e19a0a1b3e8c8fd860164df7f935216</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>PLACED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfaf3589c11ecd7d5de63db24826b74d457</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>FIXED</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>af76cc0838783b3eb3a515eb3c3e0f7bfa47be8a40f04081635fe24485ae7c6bd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>PlacementStatus</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>a29d2678343f4b712a9bbbb8f5460ec11</anchor>
      <arglist>(const Code &amp;code=UNPLACED)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>PlacementStatus</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>a121a628ab6f7a86b99acacc0d874d97b</anchor>
      <arglist>(const PlacementStatus &amp;placementstatus)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>operator const Code &amp;</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>a375d2547ed3e8a127e34b0ee3ca14ad6</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Code &amp;</type>
      <name>getCode</name>
      <anchorfile>classHurricane_1_1Instance_1_1PlacementStatus.html</anchorfile>
      <anchor>aa907067c594076ed8422bf6c949c8731</anchor>
      <arglist>() const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Instance_1_1PlacementStatus.html" title="Instance Placement Status">secInstancePStatus</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Plug</name>
    <filename>classHurricane_1_1Plug.html</filename>
    <base>Hurricane::Component</base>
    <member kind="typedef">
      <type>Component</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>a71bee60105cbd9fdc5f0f2e5b793eeca</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Instance *</type>
      <name>getInstance</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>a6ed1a11f86fbb80afacc9cc31b18a706</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Net *</type>
      <name>getMasterNet</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>a57860e49d2674dcef6ea27d79c9d2ad8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isConnected</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>a177400e23157885a76c924bf53000957</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setNet</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>af5a0448a5cb0c4956f2f1b28f9c87530</anchor>
      <arglist>(Net *net)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PlugFilter</type>
      <name>getIsConnectedFilter</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>a51bd5d04a337544709950d7cace05f0d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PlugFilter</type>
      <name>getIsUnconnectedFilter</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>af27b873ed2420329a63ea67dcc243f07</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PlugFilter</type>
      <name>getIsConnectedFilter</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>a51bd5d04a337544709950d7cace05f0d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static PlugFilter</type>
      <name>getIsUnconnectedFilter</name>
      <anchorfile>classHurricane_1_1Plug.html</anchorfile>
      <anchor>af27b873ed2420329a63ea67dcc243f07</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1Plug.html" title="Introduction">secPlugIntro</docanchor>
    <docanchor file="classHurricane_1_1Plug.html" title="Constructors &amp; Destructors">secPlugConstructors</docanchor>
    <docanchor file="classHurricane_1_1Plug.html" title="Predefined filters">secPlugPredefinedFilters</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Point</name>
    <filename>classHurricane_1_1Point.html</filename>
    <member kind="function">
      <type></type>
      <name>Point</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>a54c8ad2b1f3005ac1564c0fd7d5ef5b7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Point</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>a871672d833661cc79101d1e43d4d8325</anchor>
      <arglist>(const DbU::Unit &amp;x, const DbU::Unit &amp;y)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Point</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>a8840ca5f42bec6203058911ea0ba6cb9</anchor>
      <arglist>(const Point &amp;point)</arglist>
    </member>
    <member kind="function">
      <type>Point &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>ae3e33361927744a483d97cb7d182a1d6</anchor>
      <arglist>(const Point &amp;point)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>a2aeb5fe96fbe9324dcbc90d41ad70fb9</anchor>
      <arglist>(const Point &amp;point) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>ac6a0b7107f04913b78f96afa69e68d86</anchor>
      <arglist>(const Point &amp;point) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setX</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>adebab98c82f881b1d2e1e7680a907830</anchor>
      <arglist>(DbU::Unit x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setY</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>a14a51f177d298ccccb25066c0298a268</anchor>
      <arglist>(DbU::Unit y)</arglist>
    </member>
    <member kind="function">
      <type>Point &amp;</type>
      <name>translate</name>
      <anchorfile>classHurricane_1_1Point.html</anchorfile>
      <anchor>a86d908d60346bc15f1af4e96eddbdb19</anchor>
      <arglist>(const DbU::Unit &amp;dx, const DbU::Unit &amp;dy)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Polygon</name>
    <filename>classHurricane_1_1Polygon.html</filename>
    <base>Hurricane::Component</base>
    <member kind="typedef">
      <type>Component</type>
      <name>Super</name>
      <anchorfile>classHurricane_1_1Polygon.html</anchorfile>
      <anchor>adac4dcd1480b81e7778775540b95f81c</anchor>
      <arglist></arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Polygon *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Polygon.html</anchorfile>
      <anchor>ac248679558ff51bf509b28050027b7da</anchor>
      <arglist>(Net *, const Layer *, const std::vector&lt; Point &gt; &amp;)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Polygon.html" title="Introduction">secPolygonIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::PrivateProperty</name>
    <filename>classHurricane_1_1PrivateProperty.html</filename>
    <base>Hurricane::Property</base>
    <member kind="function">
      <type>DBo *</type>
      <name>getOwner</name>
      <anchorfile>classHurricane_1_1PrivateProperty.html</anchorfile>
      <anchor>a070ea464f4859734163b12a134e2d8f2</anchor>
      <arglist>() const</arglist>
      <docanchor file="classHurricane_1_1PrivateProperty.html" title="Destruction">secPrivatePropertyDestruction</docanchor>
    </member>
    <docanchor file="classHurricane_1_1PrivateProperty.html" title="Introduction">secPrivatePropertyIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Property</name>
    <filename>classHurricane_1_1Property.html</filename>
    <member kind="function" virtualness="pure">
      <type>virtual Name</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Property.html</anchorfile>
      <anchor>a2759e2003c15d417b925092bc253ddd1</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>onCapturedBy</name>
      <anchorfile>classHurricane_1_1Property.html</anchorfile>
      <anchor>ac7b936414d9d85bb6509100b5dd6a667</anchor>
      <arglist>(DBo *owner)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>onReleasedBy</name>
      <anchorfile>classHurricane_1_1Property.html</anchorfile>
      <anchor>a0ea7ee2089f1463c0c16e30313b54083</anchor>
      <arglist>(DBo *owner)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>destroy</name>
      <anchorfile>classHurricane_1_1Property.html</anchorfile>
      <anchor>ab60362699e6c6ea35ace45dbd1075a2f</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>destroy</name>
      <anchorfile>classHurricane_1_1Property.html</anchorfile>
      <anchor>ab60362699e6c6ea35ace45dbd1075a2f</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1Property.html" title="Introduction">secPropertyIntro</docanchor>
    <docanchor file="classHurricane_1_1Property.html" title="The Quarks">secPropertyTheQuarks</docanchor>
    <docanchor file="classHurricane_1_1Property.html" title="How that works">secPropertyHowThatWorks</docanchor>
    <docanchor file="classHurricane_1_1Property.html" title="Creation process">secPropertyCreationProcess</docanchor>
    <docanchor file="classHurricane_1_1Property.html" title="Deletion process">secPropertyDeletionProcess</docanchor>
    <docanchor file="classHurricane_1_1Property.html" title="Naming Conventions">secPropertyNaming</docanchor>
    <docanchor file="classHurricane_1_1Property.html" title="Remarks">secPropertyRemarks</docanchor>
  </compound>
  <compound kind="class">
    <name>Isobar::PyAttributesHolder</name>
    <filename>structIsobar_1_1PyAttributesHolder.html</filename>
  </compound>
  <compound kind="class">
    <name>Isobar::PyHolderProperty</name>
    <filename>classIsobar_1_1PyHolderProperty.html</filename>
    <base>Hurricane::PrivateProperty</base>
  </compound>
  <compound kind="class">
    <name>Isobar::PythonAttributes</name>
    <filename>classIsobar_1_1PythonAttributes.html</filename>
    <member kind="function" static="yes">
      <type>static PyAttributesHolder *</type>
      <name>get</name>
      <anchorfile>classIsobar_1_1PythonAttributes.html</anchorfile>
      <anchor>abc2a7357c26409151f6badeba79cdb91</anchor>
      <arglist>(const DBo *)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static int32_t</type>
      <name>delattr</name>
      <anchorfile>classIsobar_1_1PythonAttributes.html</anchorfile>
      <anchor>a703279c730fff76257f056065b7f8abd</anchor>
      <arglist>(DBo *, std::string)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>enable</name>
      <anchorfile>classIsobar_1_1PythonAttributes.html</anchorfile>
      <anchor>a4b2191b1e2f18375cbde4c9c1f80cc15</anchor>
      <arglist>(DBo *)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>disable</name>
      <anchorfile>classIsobar_1_1PythonAttributes.html</anchorfile>
      <anchor>a889d59d4e4b0133387128637f2a33e82</anchor>
      <arglist>(DBo *)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>disableAll</name>
      <anchorfile>classIsobar_1_1PythonAttributes.html</anchorfile>
      <anchor>a5bb9421104ee90ecda93c488dbd34e2e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>disableAll</name>
      <anchorfile>classIsobar_1_1PythonAttributes.html</anchorfile>
      <anchor>aaae3df93d04779f5933d9b1be3872ae2</anchor>
      <arglist>(std::string)</arglist>
    </member>
    <docanchor file="classIsobar_1_1PythonAttributes.html" title="Introduction">sPythonPropertiesIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::QuadTree</name>
    <filename>classHurricane_1_1QuadTree.html</filename>
    <member kind="function">
      <type></type>
      <name>QuadTree</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>a91303ebe7740d87429c74205181ac702</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~QuadTree</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>a3f0c6d7849185a9881bdb3b022fe1777</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Box &amp;</type>
      <name>getBoundingBox</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>a80806e2d7e99fee07d0335697ed9b82b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Gos</type>
      <name>getGos</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>a571dd774ee953dfebb3d4162f98c679c</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Gos</type>
      <name>getGosUnder</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>a6b4aa294b89c3f6b5f49388dbb985ff7</anchor>
      <arglist>(const Box &amp;area, DbU::Unit threshold=0) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isEmpty</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>a9d942a3c16a775a9ea576ef7dc753ac9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>insert</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>ac39f6a095f3b4148b36b92d2b0906f16</anchor>
      <arglist>(Go *go)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove</name>
      <anchorfile>classHurricane_1_1QuadTree.html</anchorfile>
      <anchor>af646d2864c70f6456d845c2d6a8d1785</anchor>
      <arglist>(Go *go)</arglist>
    </member>
    <docanchor file="classHurricane_1_1QuadTree.html" title="Introduction">secQuadTreeIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Quark</name>
    <filename>classHurricane_1_1Quark.html</filename>
    <base>Hurricane::DBo</base>
    <member kind="function">
      <type>const Occurrence &amp;</type>
      <name>getOccurrence</name>
      <anchorfile>classHurricane_1_1Quark.html</anchorfile>
      <anchor>a22ee192574dae1546ec17d6c549b2ca0</anchor>
      <arglist>() const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Quark.html" title="Introduction">secQuarkIntro</docanchor>
    <docanchor file="classHurricane_1_1Quark.html" title="Important">secQuarkImportant</docanchor>
    <docanchor file="classHurricane_1_1Quark.html" title="Construction and destruction">secQuarkConstructionAndDestruction</docanchor>
    <docanchor file="classHurricane_1_1Quark.html" title="Example">secQuarkExample</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Query</name>
    <filename>classHurricane_1_1Query.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>QueryFilter</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoMasterCells</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a427b951cfef3fbeb3c2baa9abc4eae83</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoTerminalCells</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a2a1f9d4cf126b86694e05152a1b04ee9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoComponents</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a7b591d72b86f94f90d212746ed8f9f56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoMarkers</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9ade01b12e4a2af3bfba0440caa557619a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoRubbers</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a3671f7b32f05ccbd1db6e6e94da040e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoExtensionGos</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a241d98f4f53e908f113669540ef4288c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoAll</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9ad3d1832e33bbdd8fa1e07ce622c984ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoMasterCells</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a427b951cfef3fbeb3c2baa9abc4eae83</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoTerminalCells</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a2a1f9d4cf126b86694e05152a1b04ee9</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoComponents</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a7b591d72b86f94f90d212746ed8f9f56</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoMarkers</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9ade01b12e4a2af3bfba0440caa557619a</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoRubbers</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a3671f7b32f05ccbd1db6e6e94da040e4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoExtensionGos</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9a241d98f4f53e908f113669540ef4288c</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>DoAll</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a003517b82eaba58104d1749cf344eaa9ad3d1832e33bbdd8fa1e07ce622c984ec</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Query</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a999eaaaf34672fb056df14629d3197d1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~Query</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>acd18d98c6bf30dd049916508a397391a</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>unsigned int</type>
      <name>getStartLevel</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a7aac7fbdc96df19e7249bf8993eb355f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>unsigned int</type>
      <name>getStopLevel</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a3544d22dbb0685208c590cef09412796</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>size_t</type>
      <name>getDepth</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>afb81080617e5b4d3c2bedf1bf8b2ebd8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Transformation &amp;</type>
      <name>getTransformation</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>aabe2c0588f95c30a3acfec8fed269be4</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Box &amp;</type>
      <name>getArea</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a666c44432d3f5717f2fee5f57a281bdd</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const BasicLayer *</type>
      <name>getBasicLayer</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>ac683152bccef813184e572806e4c14f4</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getMasterCell</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>add13f7ff193df6ce5223f9761b6cba69</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Instance *</type>
      <name>getInstance</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a459b9f175f77fce91963eeb192c6e018</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Path</type>
      <name>getPath</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>ae6be93b35a9174b2e7b656853f450021</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>hasGoCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>aeff0c9c1ef8b787a3f1460ea55db2947</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>hasMarkerCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a4f121b05f722a661b88bb8a0dc981024</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>hasRubberCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a5d49250d46dea1542bc3034f3eb1daee</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>hasExtensionGoCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>abd8ff8d187e4499e625feb12c68e9b29</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual bool</type>
      <name>hasMasterCellCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a7ebb7b16bab183cd4508dc5639bd12ab</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>goCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a59007148bd0afa0405801f341e7e4139</anchor>
      <arglist>(Go *)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>markerCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a4ad5bf076073f107189d4b7ee48f040e</anchor>
      <arglist>(Marker *)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>rubberCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>acec322581e35c1556ce706aa5ea66aa3</anchor>
      <arglist>(Rubber *)</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>extensionGoCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a75b87e969b64caaf24ec058c0d2dfa68</anchor>
      <arglist>(Go *)=0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>masterCellCallback</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>abaf97e93c7fa96469adf64f7865938b4</anchor>
      <arglist>()=0</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setQuery</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a70fce1e5b7754f1ec11097ad5b9ecfc9</anchor>
      <arglist>(Cell *cell, const Box &amp;area, const Transformation &amp;transformation, const BasicLayer *basicLayer, ExtensionSlice::Mask extensionMask, Mask filter, DbU::Unit threshold=0)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setCell</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a36378e1604e484450a3ccee0ececcff7</anchor>
      <arglist>(Cell *cell)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setArea</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>ac41de6b1535c256c4929c075769890b1</anchor>
      <arglist>(const Box &amp;area)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setTransformation</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a885360fc2f351fc3612c7dda363b5131</anchor>
      <arglist>(const Transformation &amp;transformation)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setBasicLayer</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>ac3718c4e2cd4d5e80af32558285481af</anchor>
      <arglist>(const BasicLayer *basicLayer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setExtensionMask</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>af7f83fd3aefe1b5654f9bdd3566fe0d4</anchor>
      <arglist>(ExtensionSlice::Mask mode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setFilter</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a457a0cda5ea5ad849a46aefce2514963</anchor>
      <arglist>(Mask mode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setStartLevel</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a8c4bc1bcfae942042ccb90a46b6fb510</anchor>
      <arglist>(unsigned int level)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setStopLevel</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a70359c71d7dc7e3f17d0a29c8208c92f</anchor>
      <arglist>(unsigned int level)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>doQuery</name>
      <anchorfile>classHurricane_1_1Query.html</anchorfile>
      <anchor>a2ca5bf71c7b35e14c4a64488a5e21bc6</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1Query.html" title="Introduction">secQueryIntro</docanchor>
    <docanchor file="classHurricane_1_1Query.html">secQueryParameters</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::RegularLayer</name>
    <filename>classHurricane_1_1RegularLayer.html</filename>
    <base>Hurricane::Layer</base>
    <member kind="function">
      <type>BasicLayer *</type>
      <name>getBasicLayer</name>
      <anchorfile>classHurricane_1_1RegularLayer.html</anchorfile>
      <anchor>a148c5839b544c2a0aa5d25be5958dfaf</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static RegularLayer *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1RegularLayer.html</anchorfile>
      <anchor>a6b40a35fec1c4fc168d608b8b96c8477</anchor>
      <arglist>(Technology *technology, const Name &amp;name, BasicLayer *layer)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Relation</name>
    <filename>classHurricane_1_1Relation.html</filename>
    <base>Hurricane::SharedProperty</base>
  </compound>
  <compound kind="class">
    <name>Hurricane::RoutingPad</name>
    <filename>classHurricane_1_1RoutingPad.html</filename>
    <base>Hurricane::Component</base>
    <member kind="typedef">
      <type>Component</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a53bed3713fe846a351621d2022bc6b68</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isPlacedOccurrence</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a7a85faad00cd522aac35026af43c8b98</anchor>
      <arglist>(uint32_t flags) const</arglist>
    </member>
    <member kind="function">
      <type>Occurrence</type>
      <name>getOccurrence</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a2767550364ef01c772f3270850ec052f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Occurrence</type>
      <name>getPlugOccurrence</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a30190c50577ce47727dec11f5423a85b</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual const Layer *</type>
      <name>getLayer</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a7f1e300e4148556fa223e623738d79d4</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DbU::Unit</type>
      <name>getX</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a5c9c00c648bd0d24e1a8b0876ab442df</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual DbU::Unit</type>
      <name>getY</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>aede4c04a7f893b1e5478b164b6eaae2d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Box</type>
      <name>getBoundingBox</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a2cc2894b5e1c82b725dedcf1978dc773</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Point</type>
      <name>getCenter</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>ad254b92749146a0eaaa7ed8f33fac4da</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Point</type>
      <name>getSourcePosition</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>acc706fecb615230387a73ed8a7384c8e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Point</type>
      <name>getTargetPosition</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a8de215adabb4a3330d02339c38dd6d4b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getSourceX</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>ae80e8f84f5806582905f6695f5cc43df</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getSourceY</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a75983ff3507f4cbf2aaa4e3132eac987</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getTargetX</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a3c0f5056b20515d308c2945ab692bce5</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getTargetY</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a41a9c831d668377fc0c5e628f66465a0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>translate</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a41bf66ffda0c0ceaaebc67acd72d5b36</anchor>
      <arglist>(const DbU::Unit &amp;dx, const DbU::Unit &amp;dy)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setExternalComponent</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a9f448ad4f05f6995edc4a5ab50501586</anchor>
      <arglist>(Component *)</arglist>
    </member>
    <member kind="function">
      <type>Component *</type>
      <name>setOnBestComponent</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>af842dc50d4271214514c1fd43ffb1adb</anchor>
      <arglist>(uint32_t flags)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>restorePlugOccurrence</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a1fcb0951f5f9505c6978bf498f78fce9</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static RoutingPad *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a1883e5711b5700cd7d1024f1cff6abb0</anchor>
      <arglist>(Pin *)</arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uint32_t</type>
      <name>BiggestArea</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>add72642d150e901835fa7d66d40e327d</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uint32_t</type>
      <name>HighestLayer</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a8f7993760a105713a97abdfb05eee852</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uint32_t</type>
      <name>LowestLayer</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a14c89f0c4ca6f4108b7f2ac30ab885a6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uint32_t</type>
      <name>ShowWarning</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>a8fd74358022a29aab828700c8f7347ba</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" static="yes">
      <type>static const uint32_t</type>
      <name>ComponentSelection</name>
      <anchorfile>classHurricane_1_1RoutingPad.html</anchorfile>
      <anchor>aa2db526191b29a7e094bff309e27ef4c</anchor>
      <arglist></arglist>
    </member>
    <docanchor file="classHurricane_1_1RoutingPad.html" title="Introduction">secRoutingPadIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Rubber</name>
    <filename>classHurricane_1_1Rubber.html</filename>
    <base>Hurricane::Go</base>
    <member kind="typedef">
      <type>Go</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Rubber.html</anchorfile>
      <anchor>aa74649cee7cda714020e77194af2210a</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Net *</type>
      <name>getNet</name>
      <anchorfile>classHurricane_1_1Rubber.html</anchorfile>
      <anchor>a3d4156388d0a9e53daa00cdae9732e12</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getHook</name>
      <anchorfile>classHurricane_1_1Rubber.html</anchorfile>
      <anchor>a9f7b9bc21b4df4c2bac602f045477711</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>unsigned</type>
      <name>getCount</name>
      <anchorfile>classHurricane_1_1Rubber.html</anchorfile>
      <anchor>a5fbe74e46313c0c1a264f0c7bda11e94</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Point</type>
      <name>getCenter</name>
      <anchorfile>classHurricane_1_1Rubber.html</anchorfile>
      <anchor>a124ef92401fd95ff794e381acd24e4ff</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Hooks</type>
      <name>getHooks</name>
      <anchorfile>classHurricane_1_1Rubber.html</anchorfile>
      <anchor>a6d87ff89d80fbaeeb6fea4157b92d1e3</anchor>
      <arglist>() const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Rubber.html" title="Introduction">secRubberIntro</docanchor>
    <docanchor file="classHurricane_1_1Rubber.html" title="Constructors &amp; Destructors">secRubberConstructors</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Segment</name>
    <filename>classHurricane_1_1Segment.html</filename>
    <base>Hurricane::Component</base>
    <class kind="class">Hurricane::Segment::SourceHook</class>
    <class kind="class">Hurricane::Segment::TargetHook</class>
    <member kind="typedef">
      <type>Component</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a2f616ba119bb6e9751659814bdbf0320</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getSourceHook</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>aa9d0303b444b44d7b8e47d42ac7151eb</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getTargetHook</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a0fe6cd20516eca2a0b5639ab886bd831</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Hook *</type>
      <name>getOppositeHook</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>afcd8471a2f2cfaa0e8e78a84ff7c09fc</anchor>
      <arglist>(const Hook *hook) const</arglist>
    </member>
    <member kind="function">
      <type>Component *</type>
      <name>getSource</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>aaa8954fc5948f2a881cdbc9213f9f7a9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Component *</type>
      <name>getTarget</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a1f7f13b33be3b1a20ea23b3f501296e9</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getAnchors</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>ac179f0263fee7fc71e03c9cf8c2d4e45</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Component *</type>
      <name>getOppositeAnchor</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>aa425757f271df5c58b024b0494c21588</anchor>
      <arglist>(Component *anchor) const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getWidth</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a36c7ddda39077385bd8747a8c1df726a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getHalfWidth</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a58e4abcf545ad5c89ca2854a2b4471f8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DbU::Unit</type>
      <name>getSourceX</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a0347e3bde8e2e90b05cffdaf2d048710</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DbU::Unit</type>
      <name>getSourceY</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>abf305dd4510de6fe6fae5286acbe285a</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Point</type>
      <name>getSourcePosition</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a41c4b88c35b9af0ad741205d0a8ea9c2</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DbU::Unit</type>
      <name>getTargetX</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>abba6713e109a0925c078a9785274f389</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DbU::Unit</type>
      <name>getTargetY</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a27d530abcff9742b81c4b549db161b90</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Point</type>
      <name>getTargetPosition</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>af24bee306be3461bb5dd1ba680f2a2df</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual DbU::Unit</type>
      <name>getLength</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>a9f6c42c2de0330aa6a486cdbf550cea1</anchor>
      <arglist>() const =0</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLayer</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>acd0b0cd25c824ba7f3b1ff2776c97cf1</anchor>
      <arglist>(const Layer *layer)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setWidth</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>aec203d5d3aa96150979ba532d4bd1c7d</anchor>
      <arglist>(const DbU::Unit &amp;width)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>invert</name>
      <anchorfile>classHurricane_1_1Segment.html</anchorfile>
      <anchor>aceaa61242eb7275cf9c6a39cf1868c53</anchor>
      <arglist>()</arglist>
    </member>
    <docanchor file="classHurricane_1_1Segment.html" title="Introduction">secSegmentIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::SetCollection</name>
    <filename>classHurricane_1_1SetCollection.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <base>Collection&lt; Element &gt;</base>
    <member kind="function">
      <type></type>
      <name>SetCollection</name>
      <anchorfile>classHurricane_1_1SetCollection.html</anchorfile>
      <anchor>a3ee200fd00f3a6951906209c11c03e34</anchor>
      <arglist>(const ElementSet *elementSet=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SetCollection</name>
      <anchorfile>classHurricane_1_1SetCollection.html</anchorfile>
      <anchor>a3ee200fd00f3a6951906209c11c03e34</anchor>
      <arglist>(const ElementSet *elementSet=NULL)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::SharedProperty</name>
    <filename>classHurricane_1_1SharedProperty.html</filename>
    <base>Hurricane::Property</base>
    <docanchor file="classHurricane_1_1SharedProperty.html" title="Introduction">secSharedPropertyIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Slice</name>
    <filename>classHurricane_1_1Slice.html</filename>
    <member kind="function">
      <type>Cell *</type>
      <name>getCell</name>
      <anchorfile>classHurricane_1_1Slice.html</anchorfile>
      <anchor>ae7a75f373a3d4e8878007c38688000f8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Layer *</type>
      <name>getLayer</name>
      <anchorfile>classHurricane_1_1Slice.html</anchorfile>
      <anchor>a76c011cd461e588474a22fd0026e1f8f</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Box &amp;</type>
      <name>getBoundingBox</name>
      <anchorfile>classHurricane_1_1Slice.html</anchorfile>
      <anchor>aa1a139b188879c37a8878a2353401d65</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Gos</type>
      <name>getGos</name>
      <anchorfile>classHurricane_1_1Slice.html</anchorfile>
      <anchor>abc257f5b91c01c01a618787fd73db97b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getComponents</name>
      <anchorfile>classHurricane_1_1Slice.html</anchorfile>
      <anchor>afe7c766d33e16461c3667af88e64773e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Components</type>
      <name>getComponentsUnder</name>
      <anchorfile>classHurricane_1_1Slice.html</anchorfile>
      <anchor>ada51a63690db8912eb58f1f33aa9f62c</anchor>
      <arglist>(const Box &amp;area, DbU::Unit threshold=0) const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Slice.html" title="Introduction">secSliceIntro</docanchor>
    <docanchor file="classHurricane_1_1Slice.html" title="Construction and destruction">secSliceConstructionAndDestruction</docanchor>
    <docanchor file="classHurricane_1_1Slice.html" title="Example">secSliceExample</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Segment::SourceHook</name>
    <filename>classHurricane_1_1Segment_1_1SourceHook.html</filename>
    <base>Hurricane::Hook</base>
  </compound>
  <compound kind="class">
    <name>Hurricane::StandardPrivateProperty</name>
    <filename>classHurricane_1_1StandardPrivateProperty.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <base>Hurricane::PrivateProperty</base>
    <docanchor file="classHurricane_1_1StandardPrivateProperty.html" title="Introduction">secStandardPrivatePropertyIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::StandardRelation</name>
    <filename>classHurricane_1_1StandardRelation.html</filename>
    <base>Hurricane::Relation</base>
  </compound>
  <compound kind="class">
    <name>Hurricane::StandardSharedProperty</name>
    <filename>classHurricane_1_1StandardSharedProperty.html</filename>
    <templarg></templarg>
    <base>Hurricane::SharedProperty</base>
    <docanchor file="classHurricane_1_1StandardSharedProperty.html" title="Introduction">secStandardSharedPropertyIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::SubSetCollection</name>
    <filename>classHurricane_1_1SubSetCollection.html</filename>
    <templarg></templarg>
    <base>Hurricane::Collection</base>
    <member kind="function">
      <type></type>
      <name>SubSetCollection</name>
      <anchorfile>classHurricane_1_1SubSetCollection.html</anchorfile>
      <anchor>a6da1f511e27351cdc8b56bda7fbc44e8</anchor>
      <arglist>(const Collection&lt; Type &gt; &amp;collection, const Filter&lt; Type &gt; &amp;filter)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubSetCollection</name>
      <anchorfile>classHurricane_1_1SubSetCollection.html</anchorfile>
      <anchor>ad4e0bd9554d898f3991585758dbf2aac</anchor>
      <arglist>(const SubSetCollection &amp;subSetCollection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubSetCollection</name>
      <anchorfile>classHurricane_1_1SubSetCollection.html</anchorfile>
      <anchor>a6da1f511e27351cdc8b56bda7fbc44e8</anchor>
      <arglist>(const Collection&lt; Type &gt; &amp;collection, const Filter&lt; Type &gt; &amp;filter)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubSetCollection</name>
      <anchorfile>classHurricane_1_1SubSetCollection.html</anchorfile>
      <anchor>ad4e0bd9554d898f3991585758dbf2aac</anchor>
      <arglist>(const SubSetCollection &amp;subSetCollection)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::SubTypeCollection</name>
    <filename>classHurricane_1_1SubTypeCollection.html</filename>
    <templarg></templarg>
    <templarg></templarg>
    <base>Collection&lt; SubType &gt;</base>
    <member kind="function">
      <type></type>
      <name>SubTypeCollection</name>
      <anchorfile>classHurricane_1_1SubTypeCollection.html</anchorfile>
      <anchor>a0fd6c8e781097e607b813306fd2ad677</anchor>
      <arglist>(const Collection&lt; Type &gt; *collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubTypeCollection</name>
      <anchorfile>classHurricane_1_1SubTypeCollection.html</anchorfile>
      <anchor>a09df045ff335493ac3068a865b793291</anchor>
      <arglist>(const GenericCollection&lt; Type &gt; &amp;collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubTypeCollection</name>
      <anchorfile>classHurricane_1_1SubTypeCollection.html</anchorfile>
      <anchor>a5c223a20f42ebb72d8d4be5ee99bb2d8</anchor>
      <arglist>(const SubTypeCollection &amp;subTypeCollection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubTypeCollection</name>
      <anchorfile>classHurricane_1_1SubTypeCollection.html</anchorfile>
      <anchor>a0fd6c8e781097e607b813306fd2ad677</anchor>
      <arglist>(const Collection&lt; Type &gt; *collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubTypeCollection</name>
      <anchorfile>classHurricane_1_1SubTypeCollection.html</anchorfile>
      <anchor>a09df045ff335493ac3068a865b793291</anchor>
      <arglist>(const GenericCollection&lt; Type &gt; &amp;collection)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SubTypeCollection</name>
      <anchorfile>classHurricane_1_1SubTypeCollection.html</anchorfile>
      <anchor>a5c223a20f42ebb72d8d4be5ee99bb2d8</anchor>
      <arglist>(const SubTypeCollection &amp;subTypeCollection)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Tabulation</name>
    <filename>classHurricane_1_1Tabulation.html</filename>
    <member kind="function">
      <type></type>
      <name>Tabulation</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>a59932db80223de3ba630592218cb1005</anchor>
      <arglist>(const string &amp;s=&quot;   &quot;)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Tabulation</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>a78f765f6b4fc0e629f5d3babf7a785aa</anchor>
      <arglist>(const Tabulation &amp;tabulation)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Tabulation</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>aa549d938b7534f8eae8e8e954b1f3207</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Tabulation &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>a33c4a5152580309407e3e2730f07c321</anchor>
      <arglist>(const Tabulation &amp;tabulation)</arglist>
    </member>
    <member kind="function">
      <type>Tabulation &amp;</type>
      <name>operator++</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>ad108353099b0755a45a18ec1ab6c0b7c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Tabulation</type>
      <name>operator++</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>ae609e34474086ac1b9748368d798acae</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type>Tabulation &amp;</type>
      <name>operator--</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>af95446070605eb5d2ce89e9b8b3049be</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>Tabulation</type>
      <name>operator--</name>
      <anchorfile>classHurricane_1_1Tabulation.html</anchorfile>
      <anchor>a9bdb9f81cd412ffcfb1da048b75cbd99</anchor>
      <arglist>(int)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Tabulation.html" title="Introduction">secTabulationIntro</docanchor>
    <docanchor file="classHurricane_1_1Tabulation.html" title="New global variable">secTabulationNewGlobalVariable</docanchor>
    <docanchor file="classHurricane_1_1Tabulation.html" title="Usage">secTabulationUsage</docanchor>
    <docanchor file="classHurricane_1_1Tabulation.html" title="Remark">secTabulationRemark</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Segment::TargetHook</name>
    <filename>classHurricane_1_1Segment_1_1TargetHook.html</filename>
    <base>Hurricane::Hook</base>
  </compound>
  <compound kind="class">
    <name>Hurricane::Technology</name>
    <filename>classHurricane_1_1Technology.html</filename>
    <base>Hurricane::DBo</base>
    <member kind="function">
      <type>bool</type>
      <name>isMetal</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>ae5590e455d35f76531a6feb0c0f111a2</anchor>
      <arglist>(const Layer *) const</arglist>
    </member>
    <member kind="function">
      <type>DataBase *</type>
      <name>getDataBase</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>acf836e738fba14fa493b0e08148cc3ee</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>ae466071aa1991c853ee71af12fa62d4e</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getLayer</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a4ec69c9f8f6b483885f1900c56a97b61</anchor>
      <arglist>(const Name &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>BasicLayer *</type>
      <name>getBasicLayer</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>ab096154ce9485cef02244f0037efd4fb</anchor>
      <arglist>(const Name &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>RegularLayer *</type>
      <name>getRegularLayer</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a0e93f2f749ee9b6efd30de4ef74546cc</anchor>
      <arglist>(const Name &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>ViaLayer *</type>
      <name>getViaLayer</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a9edd085c08487642dd8745b66cf40c76</anchor>
      <arglist>(const Name &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>Layers</type>
      <name>getLayers</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a4e58c5ae8e3e82d7fe1b3bb939d6a633</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>BasicLayers</type>
      <name>getBasicLayers</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a7fccff9da6604fafb90408ba56184fc0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>BasicLayers</type>
      <name>getBasicLayers</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a997457824046ea63eba51210a8e23f85</anchor>
      <arglist>(const Layer::Mask &amp;) const</arglist>
    </member>
    <member kind="function">
      <type>RegularLayers</type>
      <name>getRegularLayers</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>abffce542bc1cee054b4a09c64449f3b8</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>ViaLayers</type>
      <name>getViaLayers</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>aacde973f6a02a232a01f3f618576e1ee</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getLayer</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a2ab8d2c386bf3daeb2b93d92ecbac6b4</anchor>
      <arglist>(const Layer::Mask &amp;, bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getMetalAbove</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>af5723b08c9d289ffef8159ac2ea71b74</anchor>
      <arglist>(const Layer *, bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getMetalBelow</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>ae02123406c7362cc14413727e8689d5a</anchor>
      <arglist>(const Layer *, bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getCutAbove</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>ac7125a8eea871918e74bb295c56caceb</anchor>
      <arglist>(const Layer *, bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getCutBelow</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a3ca39dccc7e19b404181f55777e1b933</anchor>
      <arglist>(const Layer *, bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getViaBetween</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a8209708bc594a307ea39f15a39bbf196</anchor>
      <arglist>(const Layer *, const Layer *, bool useSymbolic=true) const</arglist>
    </member>
    <member kind="function">
      <type>Layer *</type>
      <name>getNthMetal</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a81a3f3e479aeb686c61a2d0fa2931f3b</anchor>
      <arglist>(int) const</arglist>
    </member>
    <member kind="function">
      <type>PhysicalRule *</type>
      <name>getUnitRule</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a6ab76e8a246a10a395d68341bca9ea96</anchor>
      <arglist>(std::string ruleName) const</arglist>
    </member>
    <member kind="function">
      <type>PhysicalRule *</type>
      <name>getPhysicalRule</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a21ef6f7507785a587e56aecc52a0c0ee</anchor>
      <arglist>(std::string ruleName, std::string layerName) const</arglist>
    </member>
    <member kind="function">
      <type>PhysicalRule *</type>
      <name>getPhysicalRule</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>aec2ce8a8195e90537e6d35cb3ba8b58f</anchor>
      <arglist>(std::string ruleName, std::string layer1Name, std::string layer2Name) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setName</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a247b75d5cbb85198cea9e5e609304cd0</anchor>
      <arglist>(const Name &amp;)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>setSymbolicLayer</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a26c12c5828acaeb33068a2899df1134b</anchor>
      <arglist>(const Layer *)</arglist>
    </member>
    <member kind="function">
      <type>PhysicalRule *</type>
      <name>addUnitRule</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a96d62a8b3eb12560a9cb778328f8a301</anchor>
      <arglist>(std::string ruleName, std::string reference)</arglist>
    </member>
    <member kind="function">
      <type>PhysicalRule *</type>
      <name>addPhysicalRule</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a267e44b205b97ff46297d16ed278a5bc</anchor>
      <arglist>(std::string ruleName, std::string reference)</arglist>
    </member>
    <member kind="function">
      <type>PhysicalRule *</type>
      <name>addPhysicalRule</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a4210936e097a774035bf52bce7d962bc</anchor>
      <arglist>(std::string ruleName, std::string layerName, std::string reference)</arglist>
    </member>
    <member kind="function">
      <type>PhysicalRule *</type>
      <name>addPhysicalRule</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a3f04a0d9fe9c76fc3c0911c76c120e00</anchor>
      <arglist>(std::string ruleName, std::string layer1Name, std::string layer2Name, std::string reference)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Technology *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Technology.html</anchorfile>
      <anchor>a64670f0d48e9460342005df52f25c152</anchor>
      <arglist>(DataBase *, const Name &amp;)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Technology.html" title="Introduction">sTechnologyIntro</docanchor>
    <docanchor file="classHurricane_1_1Technology.html" title="Using PhysicalRules">sTechnologyRules</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Transformation</name>
    <filename>classHurricane_1_1Transformation.html</filename>
    <class kind="class">Hurricane::Transformation::Orientation</class>
    <member kind="function">
      <type></type>
      <name>Transformation</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a7a28cd6c2f62898bc947dc3f41fe3bcf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Transformation</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a6bb0e856cd0374bd6f38ee835a4744c0</anchor>
      <arglist>(const DbU::Unit &amp;tx, const DbU::Unit &amp;ty, const Orientation &amp;orientation=Orientation::ID)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Transformation</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a01a6844815e536ca38d549a13b6c034f</anchor>
      <arglist>(const Point &amp;translation, const Orientation &amp;orientation=Orientation::ID)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Transformation</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>acd94913a75fd4d3624b89e50acf4962f</anchor>
      <arglist>(const Transformation &amp;transformation)</arglist>
    </member>
    <member kind="function">
      <type>Transformation &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>aad1158c828cc8ad72eb9f17804495cd2</anchor>
      <arglist>(const Transformation &amp;transformation)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator==</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a6dbb6fe21e65506f35e1ee21d09e8447</anchor>
      <arglist>(const Transformation &amp;transformation) const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>operator!=</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a7cb2ff77e4297fadbbf357d654de66a6</anchor>
      <arglist>(const Transformation &amp;transformation) const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getTx</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a08e8c1f23a73fcd1eb111c65695e848a</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getTy</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a0915bbf45c26ae91662653ce394ee203</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Point</type>
      <name>getTranslation</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a45a5133c16af3ce80ffd084b02064013</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const Orientation &amp;</type>
      <name>getOrientation</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>ad1f4de093e3b87783b2b931444acbb76</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getX</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>af3b16accc64a12c5dcd4c099115a93f3</anchor>
      <arglist>(const DbU::Unit &amp;x, const DbU::Unit &amp;y) const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getY</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a8ad883a70823c00772293549b6e611e7</anchor>
      <arglist>(const DbU::Unit &amp;x, const DbU::Unit &amp;y) const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getX</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>adcd79ae74399431387d246dce68b8d70</anchor>
      <arglist>(const Point &amp;point) const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getY</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a87152b7b585cf409950e9aba878143d5</anchor>
      <arglist>(const Point &amp;point) const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getDx</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a2c20ee8506ad770a28c2e2b91e9b0153</anchor>
      <arglist>(const DbU::Unit &amp;dx, const DbU::Unit &amp;dy) const</arglist>
    </member>
    <member kind="function">
      <type>DbU::Unit</type>
      <name>getDy</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>aa03354ff0a204e77863f392da8bb8678</anchor>
      <arglist>(const DbU::Unit &amp;dx, const DbU::Unit &amp;dy) const</arglist>
    </member>
    <member kind="function">
      <type>Point</type>
      <name>getPoint</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>aea9a0f1d1ffeb4a38accbf0c9287a93f</anchor>
      <arglist>(const DbU::Unit &amp;x, const DbU::Unit &amp;y) const</arglist>
    </member>
    <member kind="function">
      <type>Point</type>
      <name>getPoint</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a002347125ae10c408aa5e74d3c36d62f</anchor>
      <arglist>(const Point &amp;point) const</arglist>
    </member>
    <member kind="function">
      <type>Box</type>
      <name>getBox</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a1335b209ff017d8bfd6f81ae65ae6cd7</anchor>
      <arglist>(const DbU::Unit &amp;x1, const DbU::Unit &amp;y1, const DbU::Unit &amp;x2, const DbU::Unit &amp;y2) const</arglist>
    </member>
    <member kind="function">
      <type>Box</type>
      <name>getBox</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a4d3ad1601d31e05c76bda4b417445aff</anchor>
      <arglist>(const Point &amp;point1, const Point &amp;point2) const</arglist>
    </member>
    <member kind="function">
      <type>Box</type>
      <name>getBox</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a051ddd67572c50b3b8edf75f790006e2</anchor>
      <arglist>(const Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>Transformation</type>
      <name>getTransformation</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a12ba0cbe9154661e704ce0f2638f860f</anchor>
      <arglist>(const Transformation &amp;transformation) const</arglist>
    </member>
    <member kind="function">
      <type>Transformation</type>
      <name>getInvert</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>af9b37e39fb0ba02655eba41fe7779990</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>Transformation &amp;</type>
      <name>invert</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a4d49d9fde0fe04eba9e8f0c7360f2c79</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyOn</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>ad37365bfd47851ca33519bb9a05b5402</anchor>
      <arglist>(DbU::Unit &amp;x, DbU::Unit &amp;y) const</arglist>
      <docanchor file="classHurricane_1_1Transformation.html" title="Transformers">secTransformationTransformers</docanchor>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyOn</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>ae73ca0f0e10d85267ef685d94eda9bcc</anchor>
      <arglist>(Point &amp;point) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyOn</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>aa3ab2731934330107a3f2a3079f21132</anchor>
      <arglist>(Box &amp;box) const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>applyOn</name>
      <anchorfile>classHurricane_1_1Transformation.html</anchorfile>
      <anchor>a3ac2c40977ecf061b4316ecedc87918a</anchor>
      <arglist>(Transformation &amp;transformation) const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Transformation.html" title="Introduction">secTransformationIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::TransistorLayer</name>
    <filename>classHurricane_1_1TransistorLayer.html</filename>
    <base>Hurricane::Layer</base>
    <member kind="function" static="yes">
      <type>static TransistorLayer *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1TransistorLayer.html</anchorfile>
      <anchor>ac34a9a0c5056f1f483b670a1e929ed93</anchor>
      <arglist>(Technology *technology, const Name &amp;name, BasicLayer *gateLayer, BasicLayer *activeLayer, BasicLayer *diffusionLayer, BasicLayer *wellLayer)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>tstream</name>
    <filename>clasststream.html</filename>
    <member kind="function">
      <type>int</type>
      <name>getMinLevel</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a86cfc3fc7bfa7d0064d27dea74d6888d</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getMaxLevel</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>abb7ecc2e0ddeab5442d52acb2ea5fd64</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>setMinLevel</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a0dad8cbc8fc5611b788f55c75a20a88e</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>setMaxLevel</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a75cb778234d7b49d9e89c73e6efcd132</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getLevel</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a01c90e5cc80064cae20d3a4bc3320683</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>setLevel</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a37fa7894d7ae83674ee7cb5a69a4c4a5</anchor>
      <arglist>(int)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>enabled</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a01303d5c2c5cd83d06985622ca50d77b</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>enabled</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a3dab6aeefd316ad326a29cec3c2c574b</anchor>
      <arglist>(int) const</arglist>
    </member>
    <member kind="function">
      <type>tstream &amp;</type>
      <name>log</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a1fe9745dc492e891a6e765e34fa082c3</anchor>
      <arglist>(int level, int count=0)</arglist>
    </member>
    <member kind="function">
      <type>tstream &amp;</type>
      <name>tabw</name>
      <anchorfile>clasststream.html</anchorfile>
      <anchor>a5aa7a21d7b95f1bf40e68b5b13118fd0</anchor>
      <arglist>(int level, int count)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Net::Type</name>
    <filename>classHurricane_1_1Net_1_1Type.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>Code</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNDEFINED</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a973ba9d650b909a277813926d7ec4f96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOGICAL</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a8f454d4a48d8dbdd7d6341e7285b4a35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CLOCK</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a94d77dd6484c8f8524a3960d42d3974b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>POWER</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a0b7ce81772b3f4df72edb083a52b2748</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>GROUND</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a87f5f36bbfcfac211f3dff73a8e46e65</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>UNDEFINED</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a973ba9d650b909a277813926d7ec4f96</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>LOGICAL</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a8f454d4a48d8dbdd7d6341e7285b4a35</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>CLOCK</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a94d77dd6484c8f8524a3960d42d3974b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>POWER</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a0b7ce81772b3f4df72edb083a52b2748</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <name>GROUND</name>
      <anchorfile>classHurricane_1_1Net_1_1Type.html</anchorfile>
      <anchor>a2652e3299403e0f5979a848b267163a5a87f5f36bbfcfac211f3dff73a8e46e65</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::UpdateSession</name>
    <filename>classHurricane_1_1UpdateSession.html</filename>
    <base>Hurricane::SharedProperty</base>
    <docanchor file="classHurricane_1_1UpdateSession.html" title="Update Session Mechanism">secUpdateSessionMechanism</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::VectorCollection</name>
    <filename>classHurricane_1_1VectorCollection.html</filename>
    <templarg></templarg>
    <base>Collection&lt; Element &gt;</base>
    <member kind="function">
      <type></type>
      <name>VectorCollection</name>
      <anchorfile>classHurricane_1_1VectorCollection.html</anchorfile>
      <anchor>a783d1c053ef4d46040db27bba255dfb8</anchor>
      <arglist>(const ElementVector *elementVector=NULL)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>VectorCollection</name>
      <anchorfile>classHurricane_1_1VectorCollection.html</anchorfile>
      <anchor>a783d1c053ef4d46040db27bba255dfb8</anchor>
      <arglist>(const ElementVector *elementVector=NULL)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Vertical</name>
    <filename>classHurricane_1_1Vertical.html</filename>
    <base>Hurricane::Segment</base>
    <member kind="typedef">
      <type>Segment</type>
      <name>Inherit</name>
      <anchorfile>classHurricane_1_1Vertical.html</anchorfile>
      <anchor>a0132a4151899b356b157562c792294fd</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getDySource</name>
      <anchorfile>classHurricane_1_1Vertical.html</anchorfile>
      <anchor>a798671c28c5e9848a92a6d1a9765e3ce</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const DbU::Unit &amp;</type>
      <name>getDyTarget</name>
      <anchorfile>classHurricane_1_1Vertical.html</anchorfile>
      <anchor>af9ae34d224436db7c4c30f18fbd8f9a3</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setX</name>
      <anchorfile>classHurricane_1_1Vertical.html</anchorfile>
      <anchor>aaea60cb8247f4ea837f90a4532901143</anchor>
      <arglist>(const DbU::Unit &amp;x)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>translate</name>
      <anchorfile>classHurricane_1_1Vertical.html</anchorfile>
      <anchor>a15f2854bf1d5928aff717d47015a8668</anchor>
      <arglist>(const DbU::Unit &amp;dx)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Vertical *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Vertical.html</anchorfile>
      <anchor>a1ff223aa8f0b9ffe3aefc5e3ade6d34a</anchor>
      <arglist>(Net *net, const Layer *layer, const DbU::Unit &amp;x, const DbU::Unit &amp;width=0, const DbU::Unit &amp;dySource=0, const DbU::Unit &amp;dyTarget=0)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Vertical *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1Vertical.html</anchorfile>
      <anchor>aabfa1b7d21d851f6f4c278bf2d7a2ff2</anchor>
      <arglist>(Component *source, Component *target, const Layer *layer, const DbU::Unit &amp;x, const DbU::Unit &amp;width=0, const DbU::Unit &amp;dySource=0, const DbU::Unit &amp;dyTarget=0)</arglist>
    </member>
    <docanchor file="classHurricane_1_1Vertical.html" title="Introduction">secVerticalIntro</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::ViaLayer</name>
    <filename>classHurricane_1_1ViaLayer.html</filename>
    <base>Hurricane::Layer</base>
    <member kind="function" static="yes">
      <type>static ViaLayer *</type>
      <name>create</name>
      <anchorfile>classHurricane_1_1ViaLayer.html</anchorfile>
      <anchor>af2a6aa6c3a92fb1427b21662eeed7abd</anchor>
      <arglist>(Technology *technology, const Name &amp;name, BasicLayer *bottomLayer, BasicLayer *cutLayer, BasicLayer *topLayer)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::Warning</name>
    <filename>classHurricane_1_1Warning.html</filename>
    <base>Hurricane::Exception</base>
    <member kind="function">
      <type></type>
      <name>Warning</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>aea7aa9bccac13e6c15b1eb3b4741ef0d</anchor>
      <arglist>(const string &amp;reason)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Warning</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>a93bdec83d118f235def1d354938c0529</anchor>
      <arglist>(const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Warning</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>a95e98968e28550678f1ed91193f8ee5f</anchor>
      <arglist>(int code, const string &amp;reason)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Warning</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>aad9cf84a9c08fb2012462e6f38450bf0</anchor>
      <arglist>(int code, const char *format,...)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>Warning</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>a5855a8066401c0b1d1b830a3f7c6216e</anchor>
      <arglist>(const Warning &amp;warning)</arglist>
    </member>
    <member kind="function">
      <type>Warning &amp;</type>
      <name>operator=</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>a21827cacf1cb4f53d8a1963bdeb3cd50</anchor>
      <arglist>(const Warning &amp;warning)</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>getReason</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>afe26e9c4e0801d112d743d8afe8c3ac3</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>getCode</name>
      <anchorfile>classHurricane_1_1Warning.html</anchorfile>
      <anchor>a28350d25d1691b754fd8d79d1539234c</anchor>
      <arglist>() const</arglist>
    </member>
    <docanchor file="classHurricane_1_1Warning.html" title="Introduction">secWarningIntro</docanchor>
    <docanchor file="classHurricane_1_1Warning.html" title="Nouveaux types">secWarningNouveauxTypes</docanchor>
    <docanchor file="classHurricane_1_1Warning.html" title="Remark">secWarningRemark</docanchor>
  </compound>
  <compound kind="namespace">
    <name>Hurricane</name>
    <filename>namespaceHurricane.html</filename>
    <class kind="class">Hurricane::DebugSession</class>
    <class kind="class">Hurricane::Initializer</class>
    <class kind="class">Hurricane::JsonObject</class>
    <class kind="class">Hurricane::JsonStack</class>
    <class kind="class">Hurricane::Exception</class>
    <class kind="class">Hurricane::Error</class>
    <class kind="class">Hurricane::Warning</class>
    <class kind="class">Hurricane::Interruption</class>
    <class kind="class">Hurricane::GenericCollection</class>
    <class kind="class">Hurricane::SubTypeCollection</class>
    <class kind="class">Hurricane::SubSetCollection</class>
    <class kind="class">Hurricane::Collection</class>
    <class kind="class">Hurricane::ListCollection</class>
    <class kind="class">Hurricane::MapCollection</class>
    <class kind="class">Hurricane::SetCollection</class>
    <class kind="class">Hurricane::VectorCollection</class>
    <class kind="class">Hurricane::Locator</class>
    <class kind="class">Hurricane::GenericLocator</class>
    <class kind="class">Hurricane::GenericFilter</class>
    <class kind="class">Hurricane::NotFilter</class>
    <class kind="class">Hurricane::Filter</class>
    <class kind="class">Hurricane::Relation</class>
    <class kind="class">Hurricane::StandardRelation</class>
    <class kind="class">Hurricane::Tabulation</class>
    <class kind="class">Hurricane::DbU</class>
    <class kind="class">Hurricane::Point</class>
    <class kind="class">Hurricane::Box</class>
    <class kind="class">Hurricane::Interval</class>
    <class kind="class">Hurricane::Transformation</class>
    <class kind="class">Hurricane::Name</class>
    <class kind="class">Hurricane::DBo</class>
    <class kind="class">Hurricane::DataBase</class>
    <class kind="class">Hurricane::PhysicalRule</class>
    <class kind="class">Hurricane::Technology</class>
    <class kind="class">Hurricane::Layer</class>
    <class kind="class">Hurricane::BasicLayer</class>
    <class kind="class">Hurricane::RegularLayer</class>
    <class kind="class">Hurricane::ViaLayer</class>
    <class kind="class">Hurricane::ContactLayer</class>
    <class kind="class">Hurricane::DiffusionLayer</class>
    <class kind="class">Hurricane::TransistorLayer</class>
    <class kind="class">Hurricane::Library</class>
    <class kind="class">Hurricane::Entity</class>
    <class kind="class">Hurricane::Cell</class>
    <class kind="class">Hurricane::Net</class>
    <class kind="class">Hurricane::HyperNet</class>
    <class kind="class">Hurricane::Go</class>
    <class kind="class">Hurricane::Instance</class>
    <class kind="class">Hurricane::Component</class>
    <class kind="class">Hurricane::Plug</class>
    <class kind="class">Hurricane::Contact</class>
    <class kind="class">Hurricane::Pin</class>
    <class kind="class">Hurricane::Segment</class>
    <class kind="class">Hurricane::Vertical</class>
    <class kind="class">Hurricane::Horizontal</class>
    <class kind="class">Hurricane::Pad</class>
    <class kind="class">Hurricane::Polygon</class>
    <class kind="class">Hurricane::Diagonal</class>
    <class kind="class">Hurricane::Rubber</class>
    <class kind="class">Hurricane::RoutingPad</class>
    <class kind="class">Hurricane::Quark</class>
    <class kind="class">Hurricane::Property</class>
    <class kind="class">Hurricane::PrivateProperty</class>
    <class kind="class">Hurricane::StandardPrivateProperty</class>
    <class kind="class">Hurricane::SharedProperty</class>
    <class kind="class">Hurricane::StandardSharedProperty</class>
    <class kind="class">Hurricane::Hook</class>
    <class kind="class">Hurricane::Path</class>
    <class kind="class">Hurricane::Occurrence</class>
    <class kind="class">Hurricane::QuadTree</class>
    <class kind="class">Hurricane::Slice</class>
    <class kind="class">Hurricane::Query</class>
    <class kind="class">Hurricane::UpdateSession</class>
    <member kind="typedef">
      <type>GenericCollection&lt; Cell * &gt;</type>
      <name>Cells</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a8b4ab14b26f36f43d83a50294410b44a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Cell * &gt;</type>
      <name>CellLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>abd99adab3b5944a4d1ace3d0b0b34f57</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Cell * &gt;</type>
      <name>CellFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>addb0e9cd376680ecea4966516694b799</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; DBo * &gt;</type>
      <name>DBos</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0aa3882e095f9d425c253223d1c0793d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; DBo * &gt;</type>
      <name>DBoLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7d70ef7ad837859e453171feb692535c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; DBo * &gt;</type>
      <name>DBoFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a2af87173f0c45c5dc1f504d3ea2317d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Layer * &gt;</type>
      <name>Layers</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7b7200a36ab7ce8a157ddbe78b625f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Layer * &gt;</type>
      <name>LayerLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a91a93ea29be3e6658d72f9bee0da8c7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Layer * &gt;</type>
      <name>LayerFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a150e0e72c5c5609e0feb3311fa5bc127</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Library * &gt;</type>
      <name>Libraries</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a2868a53bbb0507710460ff02fab77cad</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Library * &gt;</type>
      <name>LibraryLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0477ab8ee799bb25ce9521ac16dbb6b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Library * &gt;</type>
      <name>LibraryFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a72d63f6bfd54feac2663e60430fd443d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Entity * &gt;</type>
      <name>Entities</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af50ef2888fd2a5b58b0de14cdfaabc56</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Entity * &gt;</type>
      <name>EntityLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ad3f039057bc40adfd993d83ace24fb83</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Entity * &gt;</type>
      <name>EntityFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af0fe741a1ef28dedcb26bf979b1dea5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Net * &gt;</type>
      <name>Nets</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a3404a8b17130a1824f4a281704b04df7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Net * &gt;</type>
      <name>NetLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a2911512d442f8332c3cd3a135332cc02</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Net * &gt;</type>
      <name>NetFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0dfd2c5b40325a919d139091312732e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Go * &gt;</type>
      <name>Gos</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a4456a34f3bc6766d471c3064ace19759</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Go * &gt;</type>
      <name>GoLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ab7d66a25194b15d7646c93bcc1b15fc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Go * &gt;</type>
      <name>GoFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a372aada7b76742fd900d0bb2c5398e0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Instance * &gt;</type>
      <name>Instances</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ac9436b03a2926f34ad6863deae2baadc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Instance * &gt;</type>
      <name>InstanceLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af4f7fa4dc3a2d3bdcec6f375dc5a21bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Instance * &gt;</type>
      <name>InstanceFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a889ec1441e1876d9addf89dfab32e772</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Component * &gt;</type>
      <name>Components</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7d26d99aeb5dd6d70d51bd35d2473e72</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Component * &gt;</type>
      <name>ComponentLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ad72b1998a4ff6e68326469dec9887f4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Component * &gt;</type>
      <name>ComponentFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>acbfacb3aada84aa054e587817f204e90</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Plug * &gt;</type>
      <name>Plugs</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ac8335d2057483ee7a935c15a9460c64f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Plug * &gt;</type>
      <name>PlugLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a99a5e89f593de242e24a24b632b0534e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Plug * &gt;</type>
      <name>PlugFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ad6b0bd4bdff4c52e6163b9f54e3e5c92</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Contact * &gt;</type>
      <name>Contacts</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a1e6a8ab09f688509bd727b3fee02d0d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Contact * &gt;</type>
      <name>ContactLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a244811a7f36de747884f0c1ab1cc1025</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Contact * &gt;</type>
      <name>ContactFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a57f79232601d8739370debec00f89740</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Segment * &gt;</type>
      <name>Segments</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a30748fa53a81cb597d4a13d651238716</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Segment * &gt;</type>
      <name>SegmentLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a37c8302c278e8c1c60c6ffc0222ec4c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Segment * &gt;</type>
      <name>SegmentFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a891c4a2d614e158d183dada8b0ab1747</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Vertical * &gt;</type>
      <name>Verticals</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a146e2d3d34b4035aff422f12e85345b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Vertical * &gt;</type>
      <name>VerticalLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0b7ececb547a716d7d509210a271aae4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Vertical * &gt;</type>
      <name>VerticalFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a3ac8462f707e425944df83c57835b13d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Horizontal * &gt;</type>
      <name>Horizontals</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a721e644c7d97f2f66049ab062140b855</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Horizontal * &gt;</type>
      <name>HorizontalLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7f4e07165be3dfcec8786e88370bdb67</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Horizontal * &gt;</type>
      <name>HorizontalFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a56eb48037c2e1295fafe761bf179bf01</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Pad * &gt;</type>
      <name>Pads</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>abd1f433c44d8b515e1b8a8810aea1610</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Pad * &gt;</type>
      <name>PadLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a733c2eb75cde14a8b6a2e62edfbd2792</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Pad * &gt;</type>
      <name>PadFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aeff6d8148675fc30f366906bfc652c25</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Rubber * &gt;</type>
      <name>Rubbers</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af8923abd57508cc44931a00d61b564ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Rubber * &gt;</type>
      <name>RubberLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ae4f27278e942109c3c47ad1b741f9df3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Rubber * &gt;</type>
      <name>RubberFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ae331d428a7e324593e4a47dac023b7f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Property * &gt;</type>
      <name>Properties</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>afd7bca6dad4be54b7c03b0463e6c0004</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Property * &gt;</type>
      <name>PropertyLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a91d71616b5784225dfd4296487e49c07</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Property * &gt;</type>
      <name>PropertyFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a9e98e66d188d506145a5e92045691777</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Hook * &gt;</type>
      <name>Hooks</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a9dcd9b74dc5e2b51bec7a13c25807e02</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Hook * &gt;</type>
      <name>HookLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aca3a5babe6265eb9f140112d8e8b79e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Hook * &gt;</type>
      <name>HookFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a9287de05984b91e55592a77e8d394324</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Path &gt;</type>
      <name>Pathes</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a77e8a0f11b7d0d65a47a592f7bdfd061</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Path &gt;</type>
      <name>PathLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a4077fe144cc9efa686ec63667b7dd9bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Path &gt;</type>
      <name>PathFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af42ed7a6acaba43b9d5f30b789105bc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Occurrence &gt;</type>
      <name>Occurrences</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a1912927c128eee859af62dbe4cbe0a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Occurrence &gt;</type>
      <name>OccurrenceLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aa3f9cf43b9c8f3050da0a2549bb3d64d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Occurrence &gt;</type>
      <name>OccurrenceFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a4d74662402bde091565f52b77e261560</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Slice * &gt;</type>
      <name>Slices</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aa4a7e8a563c5687621eb5e57ade1706a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Slice * &gt;</type>
      <name>SliceLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a5c3b720aae3437342e9d6c57729dc895</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Slice * &gt;</type>
      <name>SliceFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a80703f9d02b235f3291fabbf53f86d4e</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>demangle</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a93af87d1b7b19294382ba6dae51d0363</anchor>
      <arglist>(const char *symbol)</arglist>
    </member>
    <member kind="function">
      <type>string</type>
      <name>demangle</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ae4be209e8a3f2227b0c7a22246817c6f</anchor>
      <arglist>(const type_info &amp;info)</arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; DBo * &gt;</type>
      <name>DBos</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0aa3882e095f9d425c253223d1c0793d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; DBo * &gt;</type>
      <name>DBoLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7d70ef7ad837859e453171feb692535c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; DBo * &gt;</type>
      <name>DBoFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a2af87173f0c45c5dc1f504d3ea2317d9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Layer * &gt;</type>
      <name>Layers</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7b7200a36ab7ce8a157ddbe78b625f38</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Layer * &gt;</type>
      <name>LayerLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a91a93ea29be3e6658d72f9bee0da8c7b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Layer * &gt;</type>
      <name>LayerFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a150e0e72c5c5609e0feb3311fa5bc127</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Library * &gt;</type>
      <name>Libraries</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a2868a53bbb0507710460ff02fab77cad</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Library * &gt;</type>
      <name>LibraryLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0477ab8ee799bb25ce9521ac16dbb6b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Library * &gt;</type>
      <name>LibraryFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a72d63f6bfd54feac2663e60430fd443d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Entity * &gt;</type>
      <name>Entities</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af50ef2888fd2a5b58b0de14cdfaabc56</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Entity * &gt;</type>
      <name>EntityLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ad3f039057bc40adfd993d83ace24fb83</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Entity * &gt;</type>
      <name>EntityFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af0fe741a1ef28dedcb26bf979b1dea5c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Net * &gt;</type>
      <name>Nets</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a3404a8b17130a1824f4a281704b04df7</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Net * &gt;</type>
      <name>NetLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a2911512d442f8332c3cd3a135332cc02</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Net * &gt;</type>
      <name>NetFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0dfd2c5b40325a919d139091312732e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Go * &gt;</type>
      <name>Gos</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a4456a34f3bc6766d471c3064ace19759</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Go * &gt;</type>
      <name>GoLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ab7d66a25194b15d7646c93bcc1b15fc8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Go * &gt;</type>
      <name>GoFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a372aada7b76742fd900d0bb2c5398e0c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Instance * &gt;</type>
      <name>Instances</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ac9436b03a2926f34ad6863deae2baadc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Instance * &gt;</type>
      <name>InstanceLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af4f7fa4dc3a2d3bdcec6f375dc5a21bc</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Instance * &gt;</type>
      <name>InstanceFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a889ec1441e1876d9addf89dfab32e772</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Component * &gt;</type>
      <name>Components</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7d26d99aeb5dd6d70d51bd35d2473e72</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Component * &gt;</type>
      <name>ComponentLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ad72b1998a4ff6e68326469dec9887f4d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Component * &gt;</type>
      <name>ComponentFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>acbfacb3aada84aa054e587817f204e90</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Plug * &gt;</type>
      <name>Plugs</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ac8335d2057483ee7a935c15a9460c64f</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Plug * &gt;</type>
      <name>PlugLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a99a5e89f593de242e24a24b632b0534e</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Plug * &gt;</type>
      <name>PlugFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ad6b0bd4bdff4c52e6163b9f54e3e5c92</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Contact * &gt;</type>
      <name>Contacts</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a1e6a8ab09f688509bd727b3fee02d0d2</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Contact * &gt;</type>
      <name>ContactLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a244811a7f36de747884f0c1ab1cc1025</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Contact * &gt;</type>
      <name>ContactFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a57f79232601d8739370debec00f89740</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Segment * &gt;</type>
      <name>Segments</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a30748fa53a81cb597d4a13d651238716</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Segment * &gt;</type>
      <name>SegmentLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a37c8302c278e8c1c60c6ffc0222ec4c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Segment * &gt;</type>
      <name>SegmentFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a891c4a2d614e158d183dada8b0ab1747</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Vertical * &gt;</type>
      <name>Verticals</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a146e2d3d34b4035aff422f12e85345b9</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Vertical * &gt;</type>
      <name>VerticalLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a0b7ececb547a716d7d509210a271aae4</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Vertical * &gt;</type>
      <name>VerticalFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a3ac8462f707e425944df83c57835b13d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Horizontal * &gt;</type>
      <name>Horizontals</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a721e644c7d97f2f66049ab062140b855</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Horizontal * &gt;</type>
      <name>HorizontalLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a7f4e07165be3dfcec8786e88370bdb67</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Horizontal * &gt;</type>
      <name>HorizontalFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a56eb48037c2e1295fafe761bf179bf01</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Pad * &gt;</type>
      <name>Pads</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>abd1f433c44d8b515e1b8a8810aea1610</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Pad * &gt;</type>
      <name>PadLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a733c2eb75cde14a8b6a2e62edfbd2792</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Pad * &gt;</type>
      <name>PadFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aeff6d8148675fc30f366906bfc652c25</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Rubber * &gt;</type>
      <name>Rubbers</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af8923abd57508cc44931a00d61b564ad</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Rubber * &gt;</type>
      <name>RubberLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ae4f27278e942109c3c47ad1b741f9df3</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Rubber * &gt;</type>
      <name>RubberFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>ae331d428a7e324593e4a47dac023b7f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Property * &gt;</type>
      <name>Properties</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>afd7bca6dad4be54b7c03b0463e6c0004</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Property * &gt;</type>
      <name>PropertyLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a91d71616b5784225dfd4296487e49c07</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Property * &gt;</type>
      <name>PropertyFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a9e98e66d188d506145a5e92045691777</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Hook * &gt;</type>
      <name>Hooks</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a9dcd9b74dc5e2b51bec7a13c25807e02</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Hook * &gt;</type>
      <name>HookLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aca3a5babe6265eb9f140112d8e8b79e6</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Hook * &gt;</type>
      <name>HookFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a9287de05984b91e55592a77e8d394324</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Path &gt;</type>
      <name>Pathes</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a77e8a0f11b7d0d65a47a592f7bdfd061</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Path &gt;</type>
      <name>PathLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a4077fe144cc9efa686ec63667b7dd9bf</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Path &gt;</type>
      <name>PathFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>af42ed7a6acaba43b9d5f30b789105bc1</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Occurrence &gt;</type>
      <name>Occurrences</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a1912927c128eee859af62dbe4cbe0a6b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Occurrence &gt;</type>
      <name>OccurrenceLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aa3f9cf43b9c8f3050da0a2549bb3d64d</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Occurrence &gt;</type>
      <name>OccurrenceFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a4d74662402bde091565f52b77e261560</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericCollection&lt; Slice * &gt;</type>
      <name>Slices</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>aa4a7e8a563c5687621eb5e57ade1706a</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericLocator&lt; Slice * &gt;</type>
      <name>SliceLocator</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a5c3b720aae3437342e9d6c57729dc895</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>GenericFilter&lt; Slice * &gt;</type>
      <name>SliceFilter</name>
      <anchorfile>namespaceHurricane.html</anchorfile>
      <anchor>a80703f9d02b235f3291fabbf53f86d4e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>Generalities</name>
    <title>Generalities</title>
    <filename>group__Generalities.html</filename>
    <docanchor file="group__Generalities.html" title="Introduction">secGeneralitiesIntro</docanchor>
    <docanchor file="group__Generalities.html" title="Namming conventions">secGeneralitiesNammingConventions</docanchor>
    <docanchor file="group__Generalities.html" title="GetString">secGeneralitiesGetString</docanchor>
    <docanchor file="group__Generalities.html" title="Predicates">secGeneralitiesPredicates</docanchor>
    <docanchor file="group__Generalities.html" title="Inheritance">secGeneralitiesInheritance</docanchor>
    <docanchor file="group__Generalities.html" title="Trace utilities">secGeneralitiesTraceUtilities</docanchor>
    <docanchor file="group__Generalities.html" title="Remarks">secGeneralitiesRemarks</docanchor>
  </compound>
  <compound kind="group">
    <name>JsonSupport</name>
    <title>JSON Support</title>
    <filename>group__JsonSupport.html</filename>
    <class kind="class">Hurricane::JsonObject</class>
    <class kind="class">Hurricane::JsonStack</class>
    <docanchor file="group__JsonSupport.html" title="Introduction">secJsonSupportIntro</docanchor>
    <docanchor file="group__JsonSupport.html" title="JSON Additional Semantic">secJsonSemantic</docanchor>
    <docanchor file="group__JsonSupport.html" title="JSON Driver Support">secJsonDriver</docanchor>
    <docanchor file="group__JsonSupport.html" title="DBos Special Case">secJsonDriverDBo</docanchor>
    <docanchor file="group__JsonSupport.html" title="JSON Parser Support">secJsonParser</docanchor>
    <docanchor file="group__JsonSupport.html" title="JSON Array">secJsonArray</docanchor>
    <docanchor file="group__JsonSupport.html" title="Parser Stack">secJsonStack</docanchor>
    <docanchor file="group__JsonSupport.html" title="JsonObject Life Cycle">secJsonCycle</docanchor>
  </compound>
  <compound kind="group">
    <name>grpSynthHierarchy</name>
    <title>Synthetic Class Hierarchy</title>
    <filename>group__grpSynthHierarchy.html</filename>
  </compound>
  <compound kind="group">
    <name>DbUGroup</name>
    <title>DbU/Unit description</title>
    <filename>group__DbUGroup.html</filename>
    <class kind="class">Hurricane::DbU</class>
    <docanchor file="group__DbUGroup.html" title="Introduction">secDbUIntro</docanchor>
    <docanchor file="group__DbUGroup.html" title="Precision">secDbUPrecision</docanchor>
    <docanchor file="group__DbUGroup.html" title="Resolution">secDbUResolution</docanchor>
    <docanchor file="group__DbUGroup.html" title="Grids per Lambda ratio">secDbUGridsPerLamba</docanchor>
    <docanchor file="group__DbUGroup.html" title="Grid">secDbUGrid</docanchor>
    <docanchor file="group__DbUGroup.html" title="Translators">secDbUTranslators</docanchor>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>Hurricane Documentation</title>
    <filename>index.html</filename>
  </compound>
</tagfile>
