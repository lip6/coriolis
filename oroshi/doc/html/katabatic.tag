<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.9.1">
  <compound kind="class">
    <name>python::capacitormatrix::CapacitorStack</name>
    <filename>classpython_1_1capacitormatrix_1_1CapacitorStack.html</filename>
    <base>python::capacitorunit::CapacitorUnit</base>
    <member kind="function">
      <type>def</type>
      <name>__init__</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a26feb9f1bbe649dba4a43922df17fe7d</anchor>
      <arglist>(self, device, capacitance, capacitorType, abutmentBoxPosition, nets, unitCap=0, matrixDim=[1, 1], matchingMode=False, matchingScheme=[], dummyRing=False, dummyElement=False)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>setRules</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a458149b1404a0e797668330beb75b640</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__isUnitCap__</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>ababc3906986a38853f947e52f870a50c</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__isMatchingSchemeOK__</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>ada775cb99286f2386facfe173b74f8c6</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>capacitorIdOccurence</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a3a76b3e916ad8bfec0513765c297f4ed</anchor>
      <arglist>(self, capacitorIdentifier)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>create</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a142799d140cb1936225cf8e224af2b8d</anchor>
      <arglist>(self, bbMode=False)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>capacitorLine</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a67f014dfa18e1ef67be992df3e731923</anchor>
      <arglist>(self, dy, abutmentBox_spacing, matchingSchemeRowIndex=0)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>capacitorMatrix</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>af325c6856f657efc6819db6c1ed87fa0</anchor>
      <arglist>(self, abutmentBox_spacing=0)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawAbutmentBox</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a8ff901c4f22ccba4e7fc19f9ce98109e</anchor>
      <arglist>(self, abutmentBox_spacing=0)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawBottomPlatesRLayers</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>afd56ae8981043c8ef680d0c7ac0a3acb</anchor>
      <arglist>(self, bottomPlateRLayer, drawnCapacitor)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawTopPlatesRLayers</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>ac4f954eb981ca22afd05d15d24054c78</anchor>
      <arglist>(self, topPlateRLayer, drawnCapacitor)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getVerticalRoutingTrack_width</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a306f64ae38c4f512855ec9b276f47232</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getMatrixDim</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a7c13ac996df61c10e7249efa4b26a39d</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getMatchingScheme</name>
      <anchorfile>classpython_1_1capacitormatrix_1_1CapacitorStack.html</anchorfile>
      <anchor>a47fab36d1c5f43b77f6a8bed838f489c</anchor>
      <arglist>(self)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>python::capacitorunit::CapacitorUnit</name>
    <filename>classpython_1_1capacitorunit_1_1CapacitorUnit.html</filename>
    <member kind="function">
      <type>def</type>
      <name>__init__</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>ae3f080385324a5f7e92d5fa9d99dc8d4</anchor>
      <arglist>(self, device, capacitorType, abutmentBoxPosition, capacitance=0, capDim={})</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__setCapacitorPerUnit__</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a7c8aa8fda868a8ac87effa912fd95b74</anchor>
      <arglist>(self, capacitorType)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__computeCapDim__</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a3fa95cb90eed911f790274f5c90aaa67</anchor>
      <arglist>(self, capacitance, capacitorType)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__isCapacitorUnitOK__</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a2fcd6e2dddbbe5fcb09c9365cde11535</anchor>
      <arglist>(self, capDim)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>setRules</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a458149b1404a0e797668330beb75b640</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getCapacitorType</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a826bc534525cb9da119bf5e53393c4e8</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getMaximumCapWidth</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a11bb732ab03e3d6b3fb8b4e3b109c983</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getMinimumCapWidth</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>ad2f46584dd8249360127259f6c563921</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getLayers</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a2f1c438575964c1db979c40b9db805c7</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>create</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>acd7ed0a77e223a638c85d07aed261b9f</anchor>
      <arglist>(self, t, b, bbMode=False, vEnclosure_botPlate_abtBox=None, hEnclosure_botPlate_abtBox=None)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawCapacitor</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>aeef7d302c356dc7036c732e114bb1616</anchor>
      <arglist>(self, layerDict, t, b)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>computeBottomPlateCuts</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>addb554b11e23dd09df70724d5f72c740</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>computeTopPlateCuts</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>acf415dad7b8278ca474a08abd8386665</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawAbutmentBox</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>ad3ea453f04d9ec2c38eab0ee8af8047d</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawOnePlate</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a2b2756aa53569e2b59504ae6a7286653</anchor>
      <arglist>(self, layer, net, boxDimensions)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawBottomPlateCut</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a727010b51f0670072cfe620aa0fb9973</anchor>
      <arglist>(self, layer, b)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawTopPlateCut</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>aa4e6c1a2c4d502778db3d7f571cd8306</anchor>
      <arglist>(self, layer, t)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawRoutingLayers</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a786938220c3b8fbe346cc53012385c4e</anchor>
      <arglist>(self, bottomPlateLayer, topPlateLayer, t, b)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>cutMaxNumber</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>ad927ede3cda234b24660b9286a2d68df</anchor>
      <arglist>(self, width_layer, width_cut, spacing_cut, enclosure_cut)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>cutLine</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>aa453563e9c329010f89e4b566fc4768d</anchor>
      <arglist>(self, net, layer, firstCutXCenter, firstCutYCenter, width_cut, height_cut, spacing_cut, cutNumber, direction)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>cutMatrix</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>ab29fb045a7f82b35194cbb94892fd1a1</anchor>
      <arglist>(self, net, layer, firstCutXCenter, firstCutYCenter, width_cut, height_cut, spacing_cut, cutColumnNumber, cutRowNumber)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateYMax</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a11ae88c08d4e8b91d584d4a4f69c452d</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateLeftCutXMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a62e0decc5c4a2f1484743acffaf50a1a</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateLeftCutYMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a88182f71df23236f152d02855d8f92f4</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateLeftCutYMax</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a5926a16b566714d884de27edb29e86d0</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateRightCutXMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>ad3956fe34a545d496e62f0a499427e69</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateRightCutYMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a27c6ba5c13e8838ffdba1d7005d52cdc</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateRightCutYMax</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>ac90114c3fc7731a8de7a65136371759b</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBotPlateLeftRLayerXMax</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a8f1e061c67e1794bf35624c57e75c5b7</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBottomPlateRightCutYCenter</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>aed67ac964dc5d63b17a36e6a13a09b03</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBotPlateLeftRLayerXMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a85436b731b94a4addbddd2868d842976</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBotPlateRLayerYMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a1599c5468bec2b820a9d37362d21b223</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBotPlateRLayerYMax</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a61d33d28fd5e266cc84eeefabe3460a9</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBotPlateRLayerWidth</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a57d6ff2f5cf8cd47476eba0c7dd8b183</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBotPlateRightRLayerXCenter</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>adc34c9971aa055acb6ff7dde24d5ffac</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getBotPlateLeftRLayerXCenter</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a69ed3dbeb675f6c2c52586f4fb3e2e11</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getTopPlateRLayerYMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a3765ea9bb990f6f6e22aa8292158587e</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getTopPlateRLayerYMax</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>aed0bf7a26c5ee2d1baf06b55084826f2</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getTopPlateRLayerWidth</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a4b2683706e1bf6b90ada029ffc38950e</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getTopPlateRLayerXCenter</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a92c7940656b8f2940f4a39f167880ff4</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getTopPlateRLayerXMin</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a9a2ffcdaaabfec8adb1c8a67f82eec67</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>getTopPlateRLayerXMax</name>
      <anchorfile>classpython_1_1capacitorunit_1_1CapacitorUnit.html</anchorfile>
      <anchor>a279a946fe3127e06435d8eadfa62ec04</anchor>
      <arglist>(self)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>python::capacitorrouted::RoutMatchedCapacitor</name>
    <filename>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</filename>
    <base>python::capacitorvrtracks::VerticalRoutingTracks</base>
    <member kind="function">
      <type>def</type>
      <name>__init__</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>acb14d0c0bde8b81f13e2f4c773691d69</anchor>
      <arglist>(self, vRTInstance)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>route</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a84f0757a2a026ece5364ae0cc0872ef2</anchor>
      <arglist>(self, bbMode=False)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>setRules</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a458149b1404a0e797668330beb75b640</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>setLayers</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a9fd411594953ae1c5be7025f258d4354</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>computeDimensions</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>ac6ce841e559dc495eb12a6b22cf4bdf9</anchor>
      <arglist>(self, bbMode)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>computeHRoutingTrackYCenter</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>ab5ada06fee186d6a8666421263c0baa6</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>computeHRLayerYCenter</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a71bee512b44ecf046fc420bf5040fb89</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawHRoutingTracks</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a7c8e64a89e225c861bc65ec8e8634df9</anchor>
      <arglist>(self, routingTracksLayer)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawHRLayers</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>aea5cf7e1dc273d8e1dc45e5f4d421f79</anchor>
      <arglist>(self, xPlateRLayer)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawCuts</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a0d91006a08de265233844aa7d305d4d5</anchor>
      <arglist>(self, layer_hRTrack_hRLayer, layer_tracksCut, layer_topPlateCut)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawOneCut_vRoutingTrack_HRLayer</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a974575dc937c28e440dd0a252c7ef4b7</anchor>
      <arglist>(self, net, cutLayer, cutXMin, cutYMin, cutNumber)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawCuts_vRoutingTrack_hRoutingTrack</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a7fc53fb348bcaf52180b6cba1e570152</anchor>
      <arglist>(self, cutLayer, cutNumber, enclosure_cut)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__stretchTopPlates__</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a933728945d2d4a9d641ef93bf669d3b1</anchor>
      <arglist>(self, capacitor, rlayer)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__stretchTopPlateCompactCap__</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>adc7e5eab8f957ba5f80dd1e0d0ff1e50</anchor>
      <arglist>(self, net, capacitor, routingLayer, j=0)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__setStretchingDySourceDyTarget__</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>a502017c18e8b268a07ff7f861dfb0e20</anchor>
      <arglist>(self, capacitor, deltay)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>__computeConnections__</name>
      <anchorfile>classpython_1_1capacitorrouted_1_1RoutMatchedCapacitor.html</anchorfile>
      <anchor>ab7dd108892c643c3cc0a5b3118152097</anchor>
      <arglist>(self, i, j, capacitorIdentifier)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>python::stack::Stack</name>
    <filename>classpython_1_1stack_1_1Stack.html</filename>
    <member kind="function">
      <type>def</type>
      <name>__init__</name>
      <anchorfile>classpython_1_1stack_1_1Stack.html</anchorfile>
      <anchor>a818e80b2a75b3e0d06f9826d1c60ecab</anchor>
      <arglist>(self, device, NERC, NIRC)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>setWirings</name>
      <anchorfile>classpython_1_1stack_1_1Stack.html</anchorfile>
      <anchor>a4fa5741d1869983366b4ca96c3ae85b5</anchor>
      <arglist>(self, wiringSpec)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>computeDimensions</name>
      <anchorfile>classpython_1_1stack_1_1Stack.html</anchorfile>
      <anchor>a74ff17e974efecc28c792db05b5d3eb0</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>doLayout</name>
      <anchorfile>classpython_1_1stack_1_1Stack.html</anchorfile>
      <anchor>a2b936db85a32775dd3739ccfdfef636d</anchor>
      <arglist>(self, bbMode)</arglist>
    </member>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="Stack Layout">secStackLayout</docanchor>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="Gate pitch">secGatePitch</docanchor>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="Active Side Width">secActiveSideWidth</docanchor>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="H-Track Distance">secHTrackDistance</docanchor>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="BoundingBox &amp; Overall Variables">secOverallVariables</docanchor>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="Wiring Specifications">secWiringSpecs</docanchor>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="Atomic Wiring Specification">secAtomicWiring</docanchor>
    <docanchor file="classpython_1_1stack_1_1Stack.html" title="Stack Implementation Details">secStackImplDetails</docanchor>
  </compound>
  <compound kind="class">
    <name>python::capacitorvrtracks::VerticalRoutingTracks</name>
    <filename>classpython_1_1capacitorvrtracks_1_1VerticalRoutingTracks.html</filename>
    <base>python::capacitormatrix::CapacitorStack</base>
    <member kind="function">
      <type>def</type>
      <name>__setStretching__</name>
      <anchorfile>classpython_1_1capacitorvrtracks_1_1VerticalRoutingTracks.html</anchorfile>
      <anchor>a0196828def1b7ed08eb1bfe027cf4692</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>setRules</name>
      <anchorfile>classpython_1_1capacitorvrtracks_1_1VerticalRoutingTracks.html</anchorfile>
      <anchor>a458149b1404a0e797668330beb75b640</anchor>
      <arglist>(self)</arglist>
    </member>
    <member kind="function">
      <type>def</type>
      <name>drawVRoutingTracks</name>
      <anchorfile>classpython_1_1capacitorvrtracks_1_1VerticalRoutingTracks.html</anchorfile>
      <anchor>ade479990eba75cc3c94fba25082a4b28</anchor>
      <arglist>(self, vRoutingTracksLayer)</arglist>
    </member>
  </compound>
</tagfile>
