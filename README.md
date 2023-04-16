# ImNodeSoup

This is a version od https://github.com/olvb/nodesoup for dear imgui.

I have adapted nodesoup from Olivier Birot to dear imgui. I just made the changes to allow for an interactive frame rate. All credit goes to him.


![ImNodeSoup](https://user-images.githubusercontent.com/8093144/232327587-5d43ac67-ca95-402f-a280-a08d4056f22b.gif)


If you want to use it just copy the files to a dear imgui project and adjust the include path.
Add the declaration ``` extern void ShowNodeSoup();``` and call it.

There are four example graphs that you can choose with a combo box and show them with the Fruchterman-Reingold or Kamada Kawai algorithms.
You can use the mouse wheel for zoom in or zoom out and pan clickin left button.
