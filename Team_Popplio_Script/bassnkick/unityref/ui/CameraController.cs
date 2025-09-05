using UnityEngine;

public class CameraController : MonoBehaviour
{
    public Transform player;      // Reference to the player's transform
    //public float scrollSpeed = 5f; // Speed at which the background scrolls
    
    [SerializeField]
    float offset = 10f;

    private void Update()
    {
        // Keep the camera at the player's position
        Vector3 cameraPosition = new Vector3(player.position.x + offset, transform.position.y, transform.position.z);
        transform.position = cameraPosition;
    }
}
