using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.EventSystems;

public class SendPacket : MonoBehaviour, IPointerDownHandler, IPointerUpHandler
{
	public InputField ipInput;
	public InputField portInput;
	public InputField valueInput;

    public void OnPointerDown(PointerEventData eventData)
    {
    	int port;
		Int32.TryParse(portInput.text, out port);

		if (port > 0 && !string.IsNullOrEmpty(ipInput.text) && !string.IsNullOrEmpty(valueInput.text)) {
			UDPSend.SetUp(ipInput.text, port);
			UDPSend.setUDPDirective(valueInput.text);
		}
    }

    public void OnPointerUp(PointerEventData eventData)
    {
        UDPSend.stopSending();
		UDPSend.setUDPDirective("x"); // Temp, so the robot stops
    }
}
