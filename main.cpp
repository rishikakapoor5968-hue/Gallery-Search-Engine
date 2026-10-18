#include "GalleryManager.h"
#include "SearchEngine.h"
#include "PriorityRanking.h"
#include "RecommendationSystem.h"

int main() {
    GalleryManager gm;

    // 1. Single image
    gm.addImage("sunset.jpg", "Sunset View", "nature,sunset", "08-10-2026", "Evening ka view");
    // 2. Single video - NEW FEATURE
    gm.addImage("trip.mp4", "Goa Trip Video", "travel,goa", "09-10-2026", "My goa vlog");

    // 3. Multiple files at once - NEW FEATURE
    vector<string> batchFiles = {"beach.jpg", "mountain.jpg", "party.mp4", "family.jpg"};
    gm.addMultipleImages(batchFiles, "Holiday Pic", "nature,holiday", "10-10-2026", "Batch added");

    gm.viewGallery();

    // Search test
    auto result = gm.searchByDate("08-10-2026");
    cout << "\nAaj ki files: " << result.size() << endl;

    // Step 2: Build Search Engine index from gallery metadata
    SearchEngine se;
    se.buildIndex(gm.getMetadataForOtherModules());

    // Step 3: Run search menu (interactive)
    se.searchMenu();
    PriorityRanking pr;
    pr.buildRanking(gm.getMetadataForOtherModules());

    string keyword;
    cout << "\nEnter keyword for ranking: ";
    cin >> keyword;
    pr.rankImages(keyword);

    RecommendationSystem rs;
    rs.buildGraph(gm.getMetadataForOtherModules());
    string tag;
    cout << "\nEnter tag for recommendations: ";
    cin >> tag;
    rs.recommendByTag(tag);
    return 0;
}