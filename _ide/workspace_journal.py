# 2025-02-09T14:43:38.032150
import vitis

client = vitis.create_client()
client.set_workspace(path="internalandddr.vitis")

platform = client.get_component(name="platform-2")
status = platform.build()

comp = client.get_component(name="hello_world")
comp.build()

status = platform.build()

comp.build()

