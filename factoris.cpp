#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

// Point structure to store (x, y) coordinates
struct Point {
    double x;
    double y;
};

// Function to calculate squared Euclidean distance between two points
double squaredDistance(const Point& p1, const Point& p2) {
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    return dx * dx + dy * dy;
}

// Kd-tree node structure
struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;

    KDNode(const Point& p) : point(p), left(nullptr), right(nullptr) {}
};

// Comparator for sorting points along the x-coordinate
bool compareX(const Point& p1, const Point& p2) {
    return p1.x < p2.x;
}

// Comparator for sorting points along the y-coordinate
bool compareY(const Point& p1, const Point& p2) {
    return p1.y < p2.y;
}

// Function to build a kd-tree from a vector of points
KDNode* buildKDTree(std::vector<Point>& points, int depth = 0) {
    if (points.empty()) return nullptr;

    int axis = depth % 2;
    if (axis == 0) {
        std::sort(points.begin(), points.end(), compareX);
    } else {
        std::sort(points.begin(), points.end(), compareY);
    }

    int mid = points.size() / 2;
    KDNode* root = new KDNode(points[mid]);

    std::vector<Point> leftPoints(points.begin(), points.begin() + mid);
    std::vector<Point> rightPoints(points.begin() + mid + 1, points.end());

    root->left = buildKDTree(leftPoints, depth + 1);
    root->right = buildKDTree(rightPoints, depth + 1);

    return root;
}

// Function to find the geometric median using kd-tree and adaptive step sizes
Point findGeometricMedian(KDNode* root) {
    const double epsilon = 1e-6;
    Point median = root->point;
    double stepSize = std::max(std::fabs(median.x), std::fabs(median.y));

    while (stepSize > epsilon) {
        double numeratorX = 0.0;
        double numeratorY = 0.0;
        double totalWeight = 0.0;

        std::vector<KDNode*> nodesToVisit;
        nodesToVisit.push_back(root);

        while (!nodesToVisit.empty()) {
            KDNode* current = nodesToVisit.back();
            nodesToVisit.pop_back();

            double d = std::max(epsilon, squaredDistance(median, current->point));
            numeratorX += (current->point.x / d);
            numeratorY += (current->point.y / d);
            totalWeight += (1 / d);

            double orthogonalDistance;
            if (current->point.x == median.x) {
                orthogonalDistance = std::fabs(median.y - current->point.y);
            } else {
                orthogonalDistance = std::fabs(median.x - current->point.x);
            }

            if (orthogonalDistance < stepSize) {
                if (current->left != nullptr) nodesToVisit.push_back(current->left);
                if (current->right != nullptr) nodesToVisit.push_back(current->right);
            }
        }

        double prevX = median.x;
        double prevY = median.y;

        median.x = numeratorX / totalWeight;
        median.y = numeratorY / totalWeight;

        stepSize /= 2.0;

        // Check for convergence
        if (std::fabs(prevX - median.x) < epsilon && std::fabs(prevY - median.y) < epsilon) {
            break;
        }
    }

    return median;
}

int main() {
    // Example usage:
    std::vector<Point> factoryPoints = {
        {1.0, 2.0},
        {3.0, 4.0},
        {5.0, 6.0},
        {7.0, 8.0}
    };

    // Build kd-tree
    KDNode* root = buildKDTree(factoryPoints);

    // Find geometric median
    Point warehouseLocation = findGeometricMedian(root);

    std::cout << "Warehouse Location: (" << warehouseLocation.x << ", " << warehouseLocation.y << ")\n";

    // Clean up the kd-tree (optional)
    // Add a function to delete the tree nodes to free memory

    return 0;
}
