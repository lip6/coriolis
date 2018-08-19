<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="group">
    <name>graphicsGroup</name>
    <title>Graphics</title>
    <filename>group__graphicsGroup.html</filename>
    <class kind="class">Hurricane::Graphics</class>
    <docanchor file="group__graphicsGroup" title="General Structure of the Graphics Object">secGraphicsStructure</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::Graphics</name>
    <filename>classHurricane_1_1Graphics.html</filename>
    <member kind="function" static="yes">
      <type>static Graphics *</type>
      <name>getGraphics</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a4f8d6eaeef8d9ae781be929e95545f0c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>isEnabled</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a02964067103bdd4c27bbf4af91a47cf8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const QFont</type>
      <name>getFixedFont</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a30322de9fd4a042d8773465c622a1650</anchor>
      <arglist>(int weight=QFont::Normal, bool italic=false, bool underline=false, int scale=0)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const Name &amp;</type>
      <name>getGroup</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a1262f31f53c2c36440e1e05fd2da17fc</anchor>
      <arglist>(const Name &amp;key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static QColor</type>
      <name>getColor</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>abaa552c464f1a664dc77cc0c0118c35c</anchor>
      <arglist>(const Name &amp;key, const DisplayStyle::HSVr &amp;darkening=DisplayStyle::HSVr())</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static QPen</type>
      <name>getPen</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a0197c915f8d76c3798cf19ea2d878834</anchor>
      <arglist>(const Name &amp;key, const DisplayStyle::HSVr &amp;darkening=DisplayStyle::HSVr())</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static QBrush</type>
      <name>getBrush</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>ad67a43df460d5c5d5179a81d2a72c9a9</anchor>
      <arglist>(const Name &amp;key, const DisplayStyle::HSVr &amp;darkening=DisplayStyle::HSVr())</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const string &amp;</type>
      <name>getPattern</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a08111e942b095327fa6031dca7c96e8e</anchor>
      <arglist>(const Name &amp;key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static float</type>
      <name>getThreshold</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a2291edddc743edc44fdea26437f3095f</anchor>
      <arglist>(const Name &amp;key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const DisplayStyle::HSVr &amp;</type>
      <name>getDarkening</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>af34b6fe518ecb3f2959731f055ee27ff</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const ColorScale &amp;</type>
      <name>getColorScale</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a8735a458885beb3b74ad5fb77db82797</anchor>
      <arglist>(ColorScale::ScaleType)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static QColor</type>
      <name>darken</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>ab4c69a277249850a397a4c5fce31f965</anchor>
      <arglist>(const QColor &amp;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static const TextTranslator &amp;</type>
      <name>getHtmlTranslator</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a2f5e702ff1ea293edc5a89aaae1facf2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static string</type>
      <name>toHtml</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a51b3772138b33a85204abbee08898987</anchor>
      <arglist>(const string &amp;)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>addStyle</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>ac64dd095224b3f9dfbbdcfc242b11a77</anchor>
      <arglist>(DisplayStyle *displayStyle)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>setStyle</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>acb9975e354d7c802fa3064ceb800db90</anchor>
      <arglist>(const Name &amp;key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DisplayStyle *</type>
      <name>getStyle</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a2506213ff4c28b22d356e7bfbb242ed6</anchor>
      <arglist>(const Name &amp;key)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static DisplayStyle *</type>
      <name>getStyle</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>aef89b1aa2be83b61ae0838a219d2b696</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static void</type>
      <name>enable</name>
      <anchorfile>classHurricane_1_1Graphics.html</anchorfile>
      <anchor>a08e5240382c4bc97fd84b6fa0e7d8b8e</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::DisplayStyle</name>
    <filename>classHurricane_1_1DisplayStyle.html</filename>
    <member kind="function">
      <type></type>
      <name>DisplayStyle</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>ac2923e24d8a17aba8f83803a807058bd</anchor>
      <arglist>(const Name &amp;name)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~DisplayStyle</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>ada5e04aa32eba45ff5c7567cd432f0e7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getName</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>a352d4cb8afa20d621ea00d1d1bdfdc91</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const std::string &amp;</type>
      <name>getDescription</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>a735d7c6b5bf47ad83a915b5051d9e160</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const HSVr &amp;</type>
      <name>getDarkening</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>a7f2d01d9dc599a684faccb899abb73c9</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>const Name &amp;</type>
      <name>getGroup</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>adc603f19582c76a28b39d0f70541c96f</anchor>
      <arglist>(const Name &amp;key) const </arglist>
    </member>
    <member kind="function">
      <type>const std::string &amp;</type>
      <name>getPattern</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>aa408e8085a19e1a8aa622fbae0dc731a</anchor>
      <arglist>(const Name &amp;key) const </arglist>
    </member>
    <member kind="function">
      <type>QColor</type>
      <name>getColor</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>ac567b16c654fd1098b9e766602687de0</anchor>
      <arglist>(const Name &amp;key, const HSVr &amp;) const </arglist>
    </member>
    <member kind="function">
      <type>QPen</type>
      <name>getPen</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>af89bf99faf6d79c2845e621fd31e0eaf</anchor>
      <arglist>(const Name &amp;key, const HSVr &amp;) const </arglist>
    </member>
    <member kind="function">
      <type>QBrush</type>
      <name>getBrush</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>ab52b713df94da44f311ab0b388d3d07e</anchor>
      <arglist>(const Name &amp;key, const HSVr &amp;) const </arglist>
    </member>
    <member kind="function">
      <type>float</type>
      <name>getThreshold</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>adbe5d39421a1ba4de0827ef0d94b5cc3</anchor>
      <arglist>(const Name &amp;key) const </arglist>
    </member>
    <member kind="function">
      <type>const std::vector&lt; DrawingGroup * &gt; &amp;</type>
      <name>getDrawingGroups</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>a16dc39d596aafbee0c4f9d6e2c61de7d</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>DrawingStyle</type>
      <name>find</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>a592d29b7fdb5d5941119f5cfdb3b8be6</anchor>
      <arglist>(const Name &amp;key) const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>qtAllocate</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>a9288b2780ffd5fccf94a41b597ed2efd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDescription</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>ae21071635857843536511f21da5533c7</anchor>
      <arglist>(const std::string &amp;description)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>inheritFrom</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>a603dbd5d6973a8897a85513acdfda2d0</anchor>
      <arglist>(const DisplayStyle *base)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setDarkening</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>ab17b746af2c529c62a902d0cb992be34</anchor>
      <arglist>(const HSVr &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>addDrawingStyle</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>aef2473891e0de472850f95cbd71b2cf4</anchor>
      <arglist>(const Name &amp;groupKey, const Name &amp;key, const std::string &amp;pattern, int red, int green, int blue, int borderWidth, float threshold, bool goMatched=true)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static QColor</type>
      <name>darken</name>
      <anchorfile>classHurricane_1_1DisplayStyle.html</anchorfile>
      <anchor>ab19987c4a2d971163749b7ab56b5597b</anchor>
      <arglist>(const QColor &amp;color, const DisplayStyle::HSVr &amp;darkening)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::CellWidget</name>
    <filename>classHurricane_1_1CellWidget.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>ResolutionMode</name>
      <anchorfile>classHurricane_1_1CellWidget.html</anchorfile>
      <anchor>a4b81fed78775d491c169b649870d9dd7</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>Res_CellMode</name>
      <anchorfile>classHurricane_1_1CellWidget.html</anchorfile>
      <anchor>a4b81fed78775d491c169b649870d9dd7af37fd1c10fd363f0ad04a94dbccd5789</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>Res_DesignMode</name>
      <anchorfile>classHurricane_1_1CellWidget.html</anchorfile>
      <anchor>a4b81fed78775d491c169b649870d9dd7a0c7c9bd939930fe178b5b8bcd84bdc1e</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::CellPrinter</name>
    <filename>classHurricane_1_1CellPrinter.html</filename>
    <member kind="function">
      <type></type>
      <name>CellPrinter</name>
      <anchorfile>classHurricane_1_1CellPrinter.html</anchorfile>
      <anchor>a9f0d00e4832cdecb1bd4bf494b4072db</anchor>
      <arglist>(QWidget *parent=NULL)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~CellPrinter</name>
      <anchorfile>classHurricane_1_1CellPrinter.html</anchorfile>
      <anchor>a2675fd5ed2db15ea7fec703283ff1ad3</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setScreenCellWidget</name>
      <anchorfile>classHurricane_1_1CellPrinter.html</anchorfile>
      <anchor>a7d5c8c373bf63a6f80ad5df0fbd5a702</anchor>
      <arglist>(CellWidget *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setMode</name>
      <anchorfile>classHurricane_1_1CellPrinter.html</anchorfile>
      <anchor>a27a285de4f74f07f78e16cc101f05ef7</anchor>
      <arglist>(int mode)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>toPdf</name>
      <anchorfile>classHurricane_1_1CellPrinter.html</anchorfile>
      <anchor>ac595a0549c1086ab92a3bc56609246ac</anchor>
      <arglist>(QPrinter *, bool imageOnly=false)</arglist>
    </member>
    <docanchor file="classHurricane_1_1CellPrinter" title="Basic Usage">secPrinterBasicUsage</docanchor>
    <docanchor file="classHurricane_1_1CellPrinter" title="Configuration Variables">secPrinterConfiguration</docanchor>
    <docanchor file="classHurricane_1_1CellPrinter" title="Implementation details">secPrinterImplDetails</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::CellImage</name>
    <filename>classHurricane_1_1CellImage.html</filename>
    <member kind="enumeration">
      <type></type>
      <name>Flags</name>
      <anchorfile>classHurricane_1_1CellImage.html</anchorfile>
      <anchor>a5836ca863f8bc47702ca040ac8897703</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumvalue">
      <type>@</type>
      <name>ShowScale</name>
      <anchorfile>classHurricane_1_1CellImage.html</anchorfile>
      <anchor>a5836ca863f8bc47702ca040ac8897703a5463e21b19fa7d390bfe239121195879</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>CellImage</name>
      <anchorfile>classHurricane_1_1CellImage.html</anchorfile>
      <anchor>a3f73f1a3a90d4625a5e0b31711affc34</anchor>
      <arglist>(QWidget *parent=NULL)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~CellImage</name>
      <anchorfile>classHurricane_1_1CellImage.html</anchorfile>
      <anchor>af83b3547b7e72661ca28f8a8dbdd2edd</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setScreenCellWidget</name>
      <anchorfile>classHurricane_1_1CellImage.html</anchorfile>
      <anchor>a3e140125664a1088020d9bedc7de0f4e</anchor>
      <arglist>(CellWidget *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setMode</name>
      <anchorfile>classHurricane_1_1CellImage.html</anchorfile>
      <anchor>aefcd1a8382907eaf13b6aeeaae0c3a6e</anchor>
      <arglist>(int mode)</arglist>
    </member>
    <member kind="function">
      <type>QImage *</type>
      <name>toImage</name>
      <anchorfile>classHurricane_1_1CellImage.html</anchorfile>
      <anchor>a90ed9b8a07f4302cbeecf52a95ef5578</anchor>
      <arglist>(unsigned int flags=0)</arglist>
    </member>
    <docanchor file="classHurricane_1_1CellImage" title="Basic Usage">secImageBasicUsage</docanchor>
    <docanchor file="classHurricane_1_1CellImage" title="Configuration Variables">secImageConfiguration</docanchor>
    <docanchor file="classHurricane_1_1CellImage" title="Implementation details">secImageImplDetails</docanchor>
  </compound>
  <compound kind="class">
    <name>Hurricane::CellViewer</name>
    <filename>classHurricane_1_1CellViewer.html</filename>
    <member kind="function">
      <type></type>
      <name>CellViewer</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a94abd334392f43bf92548ee1153be5e1</anchor>
      <arglist>(QWidget *parent=NULL)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~CellViewer</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>ad2573bba4536bbf2b074d98f21711fe8</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>isToolInterrupted</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>ae4fb8c98e8887c41a0994c0529b53fb7</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setEnableRedrawInterrupt</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a356453d6430cc64c794e49404a8657b0</anchor>
      <arglist>(bool)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setApplicationName</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a961c789cdae39edad13f171da05f7930</anchor>
      <arglist>(const QString &amp;)</arglist>
    </member>
    <member kind="function">
      <type>Cell *</type>
      <name>getCell</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>ae80d5ed45af7a6d3441825a6d357090c</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>setCell</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a5d4707b5e364b8a3a36857103bdac45d</anchor>
      <arglist>(Cell *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>renameCell</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>ae5d3d8c58dee944d99375650c3166587</anchor>
      <arglist>(const char *)</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual Cell *</type>
      <name>getCellFromDb</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a0ea301f7e85f936c38b5cd59e3752309</anchor>
      <arglist>(const char *)</arglist>
    </member>
    <member kind="function">
      <type>CellWidget *</type>
      <name>getCellWidget</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a1d57547e30b1fb51aec2d0f254be6d3e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const CellWidget *</type>
      <name>getCellWidget</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a2bc07d97d80e3b0a387f8b39ae4b3841</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>ControllerWidget *</type>
      <name>getControllerWidget</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a153855cfa6baa5d50bf0339820c4f145</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setAnonNetSelectable</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a7e4c9aaf405e4895e1527515dff3f067</anchor>
      <arglist>(bool)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>select</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a9f5b6c901e01c53c66cbd0f347cb10be</anchor>
      <arglist>(Occurrence &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unselect</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>a48deb92c4f58f987136d116b34f6ccbe</anchor>
      <arglist>(Occurrence &amp;)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unselectAll</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>adae07707d5d4d7a6fca0ad3c44fb95d2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setLayerVisible</name>
      <anchorfile>classHurricane_1_1CellViewer.html</anchorfile>
      <anchor>ae628cca8f2c70ff80bba3a0b62e3db14</anchor>
      <arglist>(const Name &amp;layer, bool visible)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>Hurricane::InspectorWidget</name>
    <filename>classHurricane_1_1InspectorWidget.html</filename>
    <member kind="function">
      <type></type>
      <name>InspectorWidget</name>
      <anchorfile>classHurricane_1_1InspectorWidget.html</anchorfile>
      <anchor>a6071f331a8d27bf45103aa91981e4ebb</anchor>
      <arglist>(QWidget *parent=NULL)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRootRecord</name>
      <anchorfile>classHurricane_1_1InspectorWidget.html</anchorfile>
      <anchor>aebc3241d7cf15d3e2ca7478c278c6a35</anchor>
      <arglist>(Record *)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>setRootOccurrence</name>
      <anchorfile>classHurricane_1_1InspectorWidget.html</anchorfile>
      <anchor>a79914005284bdc8426c90f3440714bb4</anchor>
      <arglist>(Occurrence &amp;)</arglist>
    </member>
    <docanchor file="classHurricane_1_1InspectorWidget" title="Inspector Memory Management">secInspectorMM</docanchor>
  </compound>
  <compound kind="dir">
    <name>hurricane</name>
    <path>/dsk/l1/jpc/coriolis-2.x/src/coriolis/hurricane/src/viewer/hurricane/</path>
    <filename>dir_dadde0d0ee7514f4b9d838e8db4a8c39.html</filename>
    <dir>viewer</dir>
  </compound>
  <compound kind="dir">
    <name>src</name>
    <path>/dsk/l1/jpc/coriolis-2.x/src/coriolis/hurricane/src/</path>
    <filename>dir_8411fe88106249809ce887a3285ac2b5.html</filename>
    <dir>viewer</dir>
  </compound>
  <compound kind="dir">
    <name>viewer</name>
    <path>/dsk/l1/jpc/coriolis-2.x/src/coriolis/hurricane/src/viewer/hurricane/viewer/</path>
    <filename>dir_e61ba495830392da74308d37dc1d0e01.html</filename>
    <file>CellImage.h</file>
    <file>CellPrinter.h</file>
    <file>CellViewer.h</file>
    <file>CellWidget.h</file>
    <file>DisplayStyle.h</file>
    <file>Graphics.h</file>
    <file>InspectorWidget.h</file>
  </compound>
  <compound kind="dir">
    <name>viewer</name>
    <path>/dsk/l1/jpc/coriolis-2.x/src/coriolis/hurricane/src/viewer/</path>
    <filename>dir_d17b4bff01b823ea10853862cd33daf4.html</filename>
    <dir>hurricane</dir>
  </compound>
</tagfile>
